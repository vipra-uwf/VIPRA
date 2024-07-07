#pragma once

#include <string>

#include <mpi.h>
#include <cstddef>
#include <type_traits>

#include "vipra/special_modules/parameters.hpp"

#include "vipra/types/idx.hpp"
#include "vipra/types/parameter.hpp"
#include "vipra/types/util/result_or_void.hpp"

#include "vipra/util/all_of_type.hpp"

#include "vipra/parameter_sweep/ps_util.hpp"

namespace VIPRA {
class ParameterSweep {
 public:
  static void initialize(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    MPI_Comm_dup(MPI_COMM_WORLD, &comm);
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);
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
  template <typename sim_t, typename params_t, typename callback_t = VIPRA::VOID>
  static void run(sim_t&& sim, params_t&& params, size_t count, callback_t&& callback = VOID{}) {
    load_params(params);
    size_t localCount = sim_count(rank, size, count);

    // add the correct simulation number for the current worker
    // add, because this may be called multiple times
    sim.add_sim_id(start_sim_id(rank, size, count));

    for ( size_t i = 0; i < localCount; ++i ) {
      // run the simulation
      // if a callback is provided, call that on completion
      if constexpr ( std::is_same_v<callback_t, VIPRA::VOID> ) {
        sim.parallel_run(params);
      } else {
        // TODO(rolland): this doesn't properly warn that the callback is not being used
        if constexpr ( std::is_invocable_v<callback_t, decltype(sim.parallel_run(params))> ) {
          callback(sim.get_sim_id(), sim.parallel_run(params));
        } else if constexpr ( std::is_invocable_v<callback_t, VIPRA::idx> ) {
          sim.parallel_run(params);
          callback(sim.get_sim_id());
        } else {
          sim.parallel_run(params);
          callback();
        }
      }
    }

    // update each worker to the correct sim count
    sim.set_sim_id(count);
    MPI_Barrier(MPI_COMM_WORLD);
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
  static void master_do(auto&& func) {
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
    ~DeferedFinalize() {
      int flag = 0;
      MPI_Initialized(&flag);
      if ( flag ) MPI_Finalize();
    }
  };

  // NOLINTBEGIN
  static MPI_Comm        comm;
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
  static void load_params(params_t& params) {
    std::string serialized{};
    int         length{};

    auto& input = params.get_input();

    if ( rank == 0 ) {
      input.load();
      serialized = input.serialize();
      length = static_cast<int>(serialized.size());
    }

    MPI_Bcast(&length, 1, MPI_INT, 0, comm);

    if ( rank != 0 ) {
      serialized.resize(length);
    }

    MPI_Bcast(serialized.data(), length, MPI_CHAR, 0, comm);

    if ( rank != 0 ) {
      input.deserialize(serialized);
    }
  }
};
}  // namespace VIPRA