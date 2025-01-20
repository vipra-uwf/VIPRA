#pragma once

#include <vector>
#include "vipra/util/mpi_types.hpp"

#ifdef VIPRA_USE_MPI
#include <mpi.h>

namespace VIPRA::Util {

/**
 * @brief Gathers vectors from each node into the master node
 * 
 * @tparam data_t 
 * @param localData 
 * @return std::pair<std::vector<data_t>, std::vector<int>> combined vector, counts of elements for each (both empty if rank != 0)
 */
template <typename data_t>
[[nodiscard]] inline auto mpi_gather_all_vectors(
    std::vector<data_t> const& localData)
    -> std::pair<std::vector<data_t>, std::vector<int>>
{
  MPI_Barrier(MPI_COMM_WORLD);

  int rank{};
  int size{};
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  std::vector<int> counts;
  std::vector<int> displacements;

  if ( rank == 0 ) {
    counts.resize(size);
    displacements.resize(size);
  }

  int localSize = static_cast<int>(localData.size());
  MPI_Gather(&localSize, 1, MPI_INT, counts.data(), 1, MPI_INT, 0,
             MPI_COMM_WORLD);

  std::vector<data_t> allData;
  if ( rank == 0 ) {
    int totalSize = 0;

    for ( int i = 0; i < size; ++i ) {
      displacements[i] = totalSize;
      totalSize += counts[i];
    }

    allData.resize(totalSize);  // Allocate space for all data
  }

  // Gather all vectors into the root process
  MPI_Gatherv(localData.data(), localSize, VIPRA::Util::get_mpi_type<data_t>(),
              allData.data(), counts.data(), displacements.data(),
              VIPRA::Util::get_mpi_type<data_t>(), 0, MPI_COMM_WORLD);

  MPI_Barrier(MPI_COMM_WORLD);

  return {std::move(allData), std::move(counts)};
}

/**
   * @brief Run a function only on the master node
   * 
   * @param func 
   */
inline void master_do(auto&& func)
{
  int rank{};

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if ( rank == 0 ) {
    func();
  }
}

}  // namespace VIPRA::Util
#else
namespace VIPRA::Util {
template <typename data_t>
[[nodiscard]] inline auto mpi_gather_all_vectors(
    std::vector<data_t> const& localData)
    -> std::pair<std::vector<data_t>, std::vector<int>>
{
  static_assert(
      false,
      "Attempting to use mpi_gather_all_vectors when MPI is not enabled");
}

/**
   * @brief Run a function only on the master node
   * 
   * @param func 
   */
inline void master_do(auto&& func) { func(); }
}  // namespace VIPRA::Util
#endif