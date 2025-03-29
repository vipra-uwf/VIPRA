#pragma once

#ifdef VIPRA_TIME_SIM

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
class Timings {
 public:
  using time_t = Util::nano;

  void start_new() { _clock.start(); }
  void pause() { _clock.pause(); }
  void resume() { _clock.resume(); }
  void stop() { _times.emplace_back(_clock.stop()); }

  static void set_output_file(std::filesystem::path const& filepath)
  {
    timingPath = filepath;
  }

#ifdef VIPRA_USE_MPI
  void output_timings()
  {
    stop();

    std::vector<int64_t> timings(_times.size());
    std::transform(
        _times.begin(), _times.end(), timings.begin(),
        [](Util::nano const& time) { return static_cast<int64_t>(time.count()); });

    auto [allTimings, counts] = Util::mpi_gather_all_vectors<int64_t>(timings);

    Util::master_do([&]() { output_timings_file(allTimings, counts, timingPath); });

    MPI_Barrier(MPI_COMM_WORLD);
  }

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
        file << time_string(static_cast<time_t>(timings[start + j]));
        if ( j != counts[i] - 1 ) file << ',';
      }

      start += counts[i];
      file << '\n';
    }

    file.close();
  }
#else
  void output_timings()
  {
    std::ofstream file(timingPath, std::ios_base::app);

    if ( ! file.is_open() )
      throw std::runtime_error("Unable to open timings output file: " +
                               timingPath.string());

    file << _name << ',';

    for ( size_t timeIdx = 0; timeIdx < _times.size(); ++timeIdx ) {
      file << time_string(static_cast<time_t>(_times[timeIdx]));
      if ( timeIdx != _times.size() - 1 ) file << ',';
    }

    file << '\n';

    file.close();
  }
#endif
  Timings() = default;
  ~Timings() = default;
  Timings(Timings&&) = default;
  auto operator=(const Timings&) -> Timings& = default;
  auto operator=(Timings&&) -> Timings& = default;
  Timings(Timings const&) = default;
  explicit Timings(char const* name) : _name(name) {}

 private:
  std::string         _name;
  Util::Clock<time_t> _clock;
  std::vector<time_t> _times;

  static std::filesystem::path timingPath;
};
}  // namespace VIPRA::Util

#else

#include <filesystem>

namespace VIPRA::Util {
class Timings {
 public:
  explicit Timings(char const* /*unused*/) noexcept {}
  void start_new() const noexcept {}
  void pause() const noexcept {}
  void resume() const noexcept {}
  void stop() const noexcept {}
  void output_timings() const noexcept {}
  void output_timings(std::filesystem::path const& /*output file*/) const noexcept {}
  static void set_output_file(std::filesystem::path const& /*filepath*/) noexcept {}
};
}  // namespace VIPRA::Util
#endif
