#pragma once

#include <mpi/mpi.h>
#include <cstddef>
#include <type_traits>

#include "vipra/types/util/result_or_void.hpp"
#include "vipra/util/all_of_type.hpp"

namespace VIPRA {
class ParameterSweep {
 public:
  static void initialize(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    MPI_Comm_dup(MPI_COMM_WORLD, &comm);
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);
  }

  template <typename sim_t, typename callback_t = VIPRA::VOID>
  static void run(sim_t&& sim, size_t count, callback_t&& callback = VOID{}) {
    for (size_t i = 0; i < count; ++i) {
      sim.reconfig();

      if constexpr (std::is_same_v<callback_t, VIPRA::VOID>) {
        sim();
      } else {
        // TODO(rolland): this doesn't properly warn that the callback is not being used
        if constexpr (std::is_invocable_v<callback_t, decltype(sim())>) {
          callback(sim());
        } else {
          sim();
          callback();
        }
      }
    }
  }

  [[nodiscard]] static auto get_rank() -> int { return rank; }
  [[nodiscard]] static auto get_size() -> int { return size; }

 private:
  // NOLINTBEGIN
  static MPI_Comm comm;
  static int      rank;
  static int      size;
  // NOLINTEND
};
}  // namespace VIPRA