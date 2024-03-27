#pragma once

#include <utility>

#include "vipra/geometry/f3d.hpp"
#include "vipra/types/time.hpp"

#include "vipra/vipra_behaviors/definitions/dsl_types.hpp"
#include "vipra/vipra_behaviors/values/numeric_value.hpp"

namespace VIPRA::Behaviors {

/**
 * @brief Holds time released latches
 * 
 */
class TimedLatchCollection {
 public:
  explicit TimedLatchCollection(Behaviors::NumericValue value) : _duration(std::move(value)) {}

  /**
   * @brief Sets the size of the container
   * 
   * @param latchCnt : number of latches
   */
  void resize(VIPRA::size latchCnt) { _startTimes.resize(latchCnt, -1); }

  /**
   * @brief Sets the latch for pedestrian at pedIdx
   * @note subsequent calls do not update the latched time, until the latch is unlatched
   * 
   * @param startTime : current simulation time
   * @param pedIdx : pedestrian index to latch
   */
  void latch(VIPRA::time_s startTime, VIPRA::idx pedIdx) {
    if (_startTimes[pedIdx] == -1) {
      _startTimes[pedIdx] = startTime;
    }
  }

  [[nodiscard]] auto size() const -> VIPRA::size { return _startTimes.size(); }

  /**
   * @brief Checks if the latches duration time has passed since it was latched
   * 
   * @param currTime : current simulation time
   * @param pedIdx : pedestrian index latch to check
   * @return true : if the latch is set and the duration has not elapsed
   * @return false : if the latch is NOT set or the time has elapsed
   */
  auto check(VIPRA::time_s currTime, VIPRA::idx pedIdx) -> bool {
    if (_startTimes[pedIdx] == -1) return false;

    VIPRA::f_pnt val = _duration.value(pedIdx);
    return !(currTime - _startTimes[pedIdx] >= val);
  }

  /**
   * @brief Returns the duration for a pedestrians latch
   * 
   * @param pedIdx : pedestrian index to get duration of
   * @return VIPRA::f_pnt 
   */
  auto get_duration(VIPRA::idx pedIdx) -> VIPRA::f_pnt { return _duration.value(pedIdx); }

 private:
  Behaviors::NumericValue    _duration;
  std::vector<VIPRA::time_s> _startTimes;

  /**
   * @brief Checks if the current time is within checkTime +- timestep size
   * 
   * @param currTime : current simulation time
   * @param checkTime : middle of time range
   * @param dT : timestep size
   * @return true : if inside time range
   * @return false : if NOT inside time range
   */
  static inline constexpr auto in_time_step(VIPRA::time_s currTime, VIPRA::time_s checkTime,
                                            VIPRA::delta_t deltaT) -> bool {
    const VIPRA::delta_t left = checkTime - deltaT;
    const VIPRA::delta_t right = checkTime + deltaT;
    return (currTime > left && currTime < right);
  }
};
}  // namespace VIPRA::Behaviors
