#pragma once

#ifdef VIPRA_USE_MPI
#include <mpi.h>
#endif

#include <cstddef>
#include <string>
#include <type_traits>

#include "vipra/types/idx.hpp"
#include "vipra/types/util/result_or_void.hpp"

#include "vipra/parameter_sweep/ps_util.hpp"

#include "vipra/logging/logging.hpp"

namespace VIPRA {
class ParameterSweep {
 public:
  static void initialize(int argc, char** argv)
  {
#ifdef VIPRA_USE_MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_dup(MPI_COMM_WORLD, &comm);
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);
    Log::info("MPI Initialized, rank: {}, size: {}", rank, size);
#else
    rank = 0;
    size = 1;
#endif
  }

  /**
   * @brief Runs a parameter sweep over the worker nodes
   * 
   * @tparam sim_t 
   * @tparam params_t 
   * @tparam callback_t 
   * @param sim 
   * @param params 
   * @param count 
   * @param callback 
   */
  template <typename sim_t, typename pedinput_t, typename obsinput_t, typename params_t,
            typename callback_t = VIPRA::VOID>
  static void run(sim_t&& sim, pedinput_t&& pedInput, obsinput_t&& obsInput, params_t&& params, size_t count,
                  callback_t&& callback = VOID{})
  {
    load_params(params);
    load_inputs(pedInput);
    load_inputs(obsInput);

    size_t localCount = sim_count(rank, size, count);

    // add the correct simulation number for the current worker
    // add, because this may be called multiple times
    sim.add_sim_id(start_sim_id(rank, size, count));

    // TODO(rolland): issue #22 improve readability

    for ( size_t i = 0; i < localCount; ++i ) {
      // run the simulation
      // if a callback is provided, call that on completion
      if constexpr ( std::is_same_v<callback_t, VIPRA::VOID> ) {
        sim.parallel_run(std::forward<pedinput_t>(pedInput), std::forward<obsinput_t>(obsInput),
                         std::forward<params_t>(params));
      }
      else {
        // TODO(rolland): issue #23 this doesn't properly warn that the callback is not being used
        if constexpr ( std::is_invocable_v<callback_t,
                                           decltype(sim.parallel_run(std::forward<pedinput_t>(pedInput),
                                                                     std::forward<obsinput_t>(obsInput),
                                                                     std::forward<params_t>(params)))> ) {
          callback(sim.get_sim_id(),
                   sim.parallel_run(std::forward<pedinput_t>(pedInput), std::forward<obsinput_t>(obsInput),
                                    std::forward<params_t>(params)));
        }
        else if constexpr ( std::is_invocable_v<callback_t, VIPRA::idx> ) {
          sim.parallel_run(std::forward<pedinput_t>(pedInput), std::forward<obsinput_t>(obsInput),
                           std::forward<params_t>(params));
          callback(sim.get_sim_id());
        }
        else {
          sim.parallel_run(std::forward<pedinput_t>(pedInput), std::forward<obsinput_t>(obsInput),
                           std::forward<params_t>(params));
          callback();
        }
      }
    }

    // update each worker to the correct sim count
    sim.set_sim_id(count);

#ifdef VIPRA_USE_MPI
    MPI_Barrier(MPI_COMM_WORLD);
#endif
  }

  [[nodiscard]] static auto get_rank() -> int { return rank; }
  [[nodiscard]] static auto get_size() -> int { return size; }
  [[nodiscard]] static auto is_parallel() -> bool { return size > 1; }
  [[nodiscard]] static auto is_root() -> bool { return rank == 0; }

  /**
   * @brief Run a function only on the master node
   * 
   * @param func 
   */
  static void master_do(auto&& func)
  {
    if ( rank == 0 ) {
      func();
    }
  }

 private:
  struct DeferedFinalize {
    DeferedFinalize(DeferedFinalize const&) = default;
    DeferedFinalize(DeferedFinalize&&) = default;
    auto operator=(DeferedFinalize const&) -> DeferedFinalize& = default;
    auto operator=(DeferedFinalize&&) -> DeferedFinalize& = default;
    DeferedFinalize() = default;
    ~DeferedFinalize()
    {
#ifdef VIPRA_USE_MPI
      int flag = 0;
      MPI_Initialized(&flag);
      if ( flag ) MPI_Finalize();
#endif
    }
  };

// NOLINTBEGIN
#ifdef VIPRA_USE_MPI
  static MPI_Comm comm;
#endif
  static int             rank;
  static int             size;
  static DeferedFinalize _finalize;
  // NOLINTEND

  /**
   * @brief Load in the parameters file and broadcast them to all workers
   * 
   * @tparam params_t 
   * @param params 
   */
  template <typename params_t>
  static void load_params(params_t& params)
  {
    std::string serialized{};
    int         length{};

    auto& input = params.get_input();

    if ( rank == 0 ) {
      input.load();
      serialized = input.serialize();
      length = static_cast<int>(serialized.size());
    }

#ifdef VIPRA_USE_MPI
    MPI_Bcast(&length, 1, MPI_INT, 0, comm);

    if ( rank != 0 ) {
      serialized.resize(length);
    }

    MPI_Bcast(serialized.data(), length, MPI_CHAR, 0, comm);

    if ( rank != 0 ) {
      input.parse(serialized);
    }
#endif
  }

  /**
   * @brief Load in the pedestrian or obstacle files and broadcast them to all workers
   * 
   * @tparam params_t 
   * @param params 
   */
  template <typename input_t>
  static void load_inputs(input_t& input)
  {
    std::string serialized{};
    int         length{};

    if ( rank == 0 ) {
      input.load();
      serialized = input.serialize();
      length = static_cast<int>(serialized.size());
    }

#ifdef VIPRA_USE_MPI
    MPI_Bcast(&length, 1, MPI_INT, 0, comm);

    if ( rank != 0 ) {
      serialized.resize(length);
    }

    MPI_Bcast(serialized.data(), length, MPI_CHAR, 0, comm);

    if ( rank != 0 ) {
      input.parse(serialized);
    }
#endif
  }
};
}  // namespace VIPRA
