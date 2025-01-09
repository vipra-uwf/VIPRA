#pragma once

#include <chrono>

namespace VIPRA::Util {
using nano = std::chrono::nanoseconds;
using micro = std::chrono::microseconds;
using milli = std::chrono::milliseconds;
using seconds = std::chrono::seconds;

template <typename time_t>
[[nodiscard]] inline auto time_string(time_t time) -> std::string
{
  auto minutes = std::chrono::duration_cast<std::chrono::minutes>(time);
  time -= minutes;
  auto seconds = std::chrono::duration_cast<std::chrono::seconds>(time);
  time -= seconds;
  auto milliseconds =
      std::chrono::duration_cast<std::chrono::milliseconds>(time);
  time -= milliseconds;
  auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(time);

  std::string timeStr;

  if ( minutes.count() > 0 ) timeStr += std::to_string(minutes.count()) += 'm';
  if ( seconds.count() > 0 ) timeStr += std::to_string(seconds.count()) += 's';
  if ( milliseconds.count() > 0 )
    timeStr += std::to_string(milliseconds.count()) += "ms";
  if ( nanoseconds.count() > 0 )
    timeStr += std::to_string(nanoseconds.count()) += "ns";

  if ( timeStr.empty() ) timeStr = "00";

  return timeStr;
}

/**
 * @brief Clock to time execution time
 * 
 * @tparam time_t : time scale
 * @tparam clock_t : type of clock to use
 */
template <typename time_t = seconds,
          typename clock_t = std::chrono::steady_clock>
class Clock {
 public:
  /**
   * @brief Returns the current time
   * 
   * @return time_t 
   */
  [[nodiscard]] inline auto now() noexcept -> time_t { return clock_t::now(); }

  /**
   * @brief Starts the timer
   * 
   */
  inline void start() noexcept
  {
    _running = true;
    _runningTime = time_t::zero();
    _start = clock_t::now();
  }

  [[nodiscard]] inline auto is_running() const noexcept -> bool
  {
    return _running;
  }

  /**
   * @brief Pauses the timer on the clock
   * 
   */
  void pause() noexcept
  {
    if ( ! _running ) return;

    _runningTime += std::chrono::duration_cast<time_t>(clock_t::now() - _start);
    _running = false;
  }

  /**
   * @brief Continues the timer after pausing
   * 
   */
  void resume() noexcept
  {
    if ( _running ) return;

    _running = true;
    _start = clock_t::now();
  }

  /**
   * @brief Returns the total accumulated time on the clock
   * 
   * @return time_t 
   */
  [[nodiscard]] inline auto running_time() noexcept -> time_t
  {
    if ( ! _running ) return _runningTime;

    return std::chrono::duration_cast<time_t>(clock_t::now() - _start) +
           _runningTime;
  }

  /**
   * @brief Resets the clock, Returns the time since it was started
   * 
   * @return time_t 
   */
  inline auto stop() noexcept -> time_t
  {
    time_t duration;
    if ( _running ) {
      duration = std::chrono::duration_cast<time_t>(clock_t::now() - _start) +
                 _runningTime;
    }
    else {
      duration = _runningTime;
    }

    _running = false;
    _runningTime = time_t::zero();

    return duration;
  }

  static constexpr time_t ZERO = time_t::zero();

 private:
  bool                             _running = false;
  std::chrono::time_point<clock_t> _start;
  time_t                           _runningTime;
};
}  // namespace VIPRA::Util
