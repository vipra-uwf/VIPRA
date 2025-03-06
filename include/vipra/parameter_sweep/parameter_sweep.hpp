#pragma once

#ifdef VIPRA_USE_MPI
#include <mpi.h>
#endif

#include <cstddef>
#include <string>
#include <type_traits>

#include "vipra/logging/logging.hpp"
#include "vipra/modules/serializable.hpp"
#include "vipra/parameter_sweep/ps_util.hpp"
#include "vipra/simulation/sim_type.hpp"
#include "vipra/special_modules/parameters.hpp"
#include "vipra/types/util/result_or_void.hpp"
#include "vipra/util/timing.hpp"

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
  static void run(Simulation& sim, std::string const& pedPath, std::string const& mapPath,
                  std::string const& paramsPath, size_t count, auto&& callback = VOID{})
  {
    Parameters params;

    _mpiTimings.start_new();
    _mpiTimings.pause();

    _inputTimings.start_new();
    load_inputs(params.get_input(), paramsPath);
    // disseminate_input(params.get_input());
    load_inputs(sim.get_map_input(), mapPath);
    if ( ! pedPath.empty() ) load_inputs(sim.get_ped_input(), pedPath);
    _inputTimings.stop();

    size_t localCount = sim_count(rank, size, count);

    // add the correct simulation number for the current worker
    // add, because this may be called multiple times
    sim.add_sim_id(start_sim_id(rank, size, count));

    for ( size_t i = 0; i < localCount; ++i ) {
      _timings.start_new();
      // run the simulation
      // if a callback is provided, call that on completion
      if constexpr ( std::is_same_v<decltype(callback), VIPRA::VOID> ) {
        sim.run_sim(params);
      }
      else {
        sim.run_sim(params);
        callback(sim.get_sim_id());
      }
      _timings.stop();
    }

    // update each worker to the correct sim count
    sim.set_sim_id(count);

    _timings.output_timings();

#ifdef VIPRA_USE_MPI
    _mpiTimings.resume();
    MPI_Barrier(MPI_COMM_WORLD);
    _mpiTimings.stop();

    _mpiTimings.output_timings();
#endif
  }

  [[nodiscard]] static auto get_rank() -> int { return rank; }
  [[nodiscard]] static auto get_size() -> int { return size; }
  [[nodiscard]] static auto is_parallel() -> bool { return size > 1; }
  [[nodiscard]] static auto is_root() -> bool { return rank == 0; }

 private:
  struct DeferredFinalize {
    DeferredFinalize(DeferredFinalize const&) = default;
    DeferredFinalize(DeferredFinalize&&) = default;
    auto operator=(DeferredFinalize const&) -> DeferredFinalize& = default;
    auto operator=(DeferredFinalize&&) -> DeferredFinalize& = default;
    DeferredFinalize() = default;
    ~DeferredFinalize()
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

  static Util::Timings _timings;
  static Util::Timings _mpiTimings;
  static Util::Timings _inputTimings;

  static int              rank;
  static int              size;
  static DeferredFinalize _finalize;
  // NOLINTEND

  /**
   * @brief Load in the pedestrian or map files and broadcast them to all workers
   * 
   * @tparam params_t 
   * @param params 
   */
  template <typename input_t>
  static void load_inputs(input_t& input, std::string const& filepath)
  {
    input.load(filepath);
  }

  static void disseminate_input(Modules::Serializable& input)
  {
    std::string serialized{};
    int         length{};
    if ( rank == 0 ) {
      serialized = input.serialize();
      length = static_cast<int>(serialized.size());
    }
#ifdef VIPRA_USE_MPI
    _mpiTimings.resume();
    MPI_Bcast(&length, 1, MPI_INT, 0, comm);

    if ( rank != 0 ) {
      serialized.resize(length);
    }

    MPI_Bcast(serialized.data(), length, MPI_CHAR, 0, comm);
    _mpiTimings.pause();

    if ( rank != 0 ) {
      input.parse(serialized);
    }
#endif
  }
};
}  // namespace VIPRA
