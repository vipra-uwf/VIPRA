#pragma once

#include <chrono>

namespace VIPRA::Util {
using nano = std::chrono::nanoseconds;
using milli = std::chrono::milliseconds;
using seconds = std::chrono::seconds;

// TODO(rolland): add to documentation

/**
 * @brief Clock to time execution time
 * 
 * @tparam time_t : time scale
 * @tparam clock_t : type of clock to use
 */
template <typename time_t = seconds, typename clock_t = std::chrono::steady_clock>
class Clock {
 public:
  /**
   * @brief Returns the current time
   * 
   * @return time_t 
   */
  [[nodiscard]] inline auto now() -> time_t { return clock_t::now(); }

  /**
   * @brief Starts the timer
   * 
   */
  inline void start() {
    _running = true;
    _start = clock_t::now();
  }

  /**
   * @brief Returns the time since it was started
   * 
   * @return time_t 
   */
  [[nodiscard]] inline auto click() -> time_t {
    if (!_running) return ZERO;
    return std::chrono::duration_cast<time_t>(clock_t::now() - _start);
  }

  /**
   * @brief Resets the clock, Returns the time since it was started
   * 
   * @return time_t 
   */
  [[nodiscard]] inline auto stop() -> time_t {
    if (!_running) return ZERO;
    auto duration = std::chrono::duration_cast<time_t>(clock_t::now() - _start);
    _running = false;
    return duration;
  }

  static constexpr time_t ZERO = time_t::zero();

 private:
  bool                             _running = false;
  std::chrono::time_point<clock_t> _start;
};
}  // namespace VIPRA::Util
