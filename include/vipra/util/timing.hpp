#pragma once

#include <algorithm>
#ifdef VIPRA_USE_MPI
#include <mpi.h>
#endif

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <vector>

#include "vipra/util/clock.hpp"

namespace VIPRA::Util {

#ifdef VIPRA_TIME_SIM

class Timings {
 public:
  explicit Timings(char const* name) : _name(name) {}
  void start_new() { _clock.start(); }
  void stop() { _times.emplace_back(_clock.stop()); }

#ifdef VIPRA_USE_MPI
  void output_timings(std::filesystem::path const& filepath)
  {
    if ( filepath.empty() ) return;

    int rank{};
    int size{};
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<int64_t> timings(_times.size());
    std::transform(_times.begin(), _times.end(), timings.begin(),
                   [](Util::milli const& time) {
                     return static_cast<int64_t>(time.count());
                   });

    std::vector<int> counts;
    std::vector<int> displacements;

    if ( rank == 0 ) {
      counts.resize(size);
      displacements.resize(size);
    }

    int localSize = static_cast<int>(timings.size());
    MPI_Gather(&localSize, 1, MPI_INT, counts.data(), 1, MPI_INT, 0,
               MPI_COMM_WORLD);

    if ( rank == 0 ) {
      int totalSize = 0;

      for ( int i = 0; i < size; ++i ) {
        displacements[i] = totalSize;
        totalSize += counts[i];
      }

      timings.resize(totalSize);  // Allocate space for all data
    }

    // Gather all vectors into the root process
    MPI_Gatherv(timings.data(), localSize, MPI_INT64_T, timings.data(),
                counts.data(), displacements.data(), MPI_INT64_T, 0,
                MPI_COMM_WORLD);

    if ( rank == 0 ) output_timings_file(timings, counts, filepath);
  }
#else
  void output_timings(std::filesystem::path const& filepath)
  {
    if ( filepath.empty() ) return;

    std::ofstream file(filepath);

    if ( ! file.is_open() )
      throw std::runtime_error("Unable to open timings output file: " +
                               filepath.string());

    file << _name << ',';

    for ( size_t timeIdx = 0; timeIdx < _times.size(); ++timeIdx ) {
      file << _times[timeIdx].count();
      if ( timeIdx != _times.size() - 1 ) file << ',';
    }

    file << '\n';

    file.close();
  }
#endif
 private:
  std::string              _name;
  Util::Clock<Util::milli> _clock;
  std::vector<Util::milli> _times;

#ifdef VIPRA_USE_MPI
  void output_timings_file(std::vector<int64_t> const&  timings,
                           std::vector<int> const&      counts,
                           std::filesystem::path const& filepath)
  {
    std::ofstream file(filepath);

    if ( ! file.is_open() )
      throw std::runtime_error("Unable to open timings output file: " +
                               filepath.string());

    size_t start{0};

    for ( int i = 0; i < counts.size(); ++i ) {
      file << _name << i << ',';

      for ( int j = 0; j < counts[i]; ++j ) {
        file << timings[start + j] << ',';
      }

      start = counts[i];
      file << '\n';
    }

    file.close();
  }
#endif
};

#else

class Timings {
 public:
  explicit Timings(char const*) {}
  void start_new() const noexcept {}
  void stop() const noexcept {}
  void output_timings(
      std::filesystem::path const& /*output file*/) const noexcept
  {
  }
};

#endif
}  // namespace VIPRA::Util