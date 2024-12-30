#pragma once

#include <algorithm>
#include <ios>

#ifdef VIPRA_USE_MPI
#include <mpi.h>
#endif

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <vector>

#include "vipra/util/clock.hpp"
#include "vipra/util/mpi_util.hpp"

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

    std::vector<int64_t> timings(_times.size());
    std::transform(_times.begin(), _times.end(), timings.begin(),
                   [](Util::milli const& time) {
                     return static_cast<int64_t>(time.count());
                   });

    auto [allTimings, counts] = Util::mpi_gather_all_vectors<int64_t>(timings);

    Util::master_do(
        [&]() { output_timings_file(allTimings, counts, filepath); });

    MPI_Barrier(MPI_COMM_WORLD);
  }
#else
  void output_timings(std::filesystem::path const& filepath)
  {
    if ( filepath.empty() ) return;

    std::ofstream file(filepath, std::ios_base::app);

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
    std::ofstream file(filepath, std::ios_base::app);

    if ( ! file.is_open() )
      throw std::runtime_error("Unable to open timings output file: " +
                               filepath.string());

    size_t start{0};

    for ( int i = 0; i < counts.size(); ++i ) {
      file << _name << i << ',';

      for ( int j = 0; j < counts[i]; ++j ) {
        file << timings[start + j] << ',';
      }

      start += counts[i];
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