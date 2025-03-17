
#pragma once

#include <tuple>
#include <type_traits>
#include <vector>

#include "badl/definitions/time.hpp"

namespace BADL {

template <typename... stimuli_ts>
class Environment {
 public:
  // TODO(rolland): add in duration to stimuli sources

  template <typename stimuli_t>
  void add_source(stimuli_t const& stimulus)
  {
    get<stimuli_t>().emplace_back(stimulus);
  }
  template <typename stimuli_t>
  void add_source(stimuli_t&& stimulus)
  {
    get<stimuli_t>().emplace_back(std::forward<stimuli_t>(stimulus));
  }

  template <typename stimuli_t>
  [[nodiscard]] auto all_sources() const -> std::vector<stimuli_t> const&
  {
    return std::get<std::vector<std::remove_cvref_t<stimuli_t>>>(
        _stimuliSources);
  }

  auto get_current_time() -> BADL::time { return _time; }
  void add_time(BADL::time deltaT) { _time += deltaT; }

 private:
  BADL::time                             _time;
  std::tuple<std::vector<stimuli_ts>...> _stimuliSources;

  template <typename stimulus_t>
  [[nodiscard]] auto get() noexcept -> std::vector<stimulus_t>&
  {
    return std::get<std::vector<stimulus_t>>(_stimuliSources);
  }

  template <typename stimulus_t>
  [[nodiscard]] auto get() const noexcept -> std::vector<stimulus_t> const&
  {
    return std::get<std::vector<stimulus_t>>(_stimuliSources);
  }
};

}  // namespace BADL