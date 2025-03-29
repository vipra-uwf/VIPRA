#pragma once

#include "vipra/vipra_behaviors/conditions/sub_condition.hpp"
#include "vipra/vipra_behaviors/util/class_types.hpp"
#include "vipra/vipra_behaviors/values/numeric_value.hpp"

namespace VIPRA::Behaviors {
/**
 * @brief SubCondition for target attribute to equal a value
 * 
 */
class SubConditionElapsedTimeFromEvent {
  NON_DEFAULT_CONSTRUCTIBLE(SubConditionElapsedTimeFromEvent)
  COPYABLE(SubConditionElapsedTimeFromEvent)
  MOVEABLE(SubConditionElapsedTimeFromEvent)
 public:
  SubConditionElapsedTimeFromEvent(Behaviors::NumericValue time, VIPRA::idx event)
      : _event(event), _requiredTime(std::move(time))
  {
  }

  void operator()(Simpack pack, const VIPRA::idxVec&                        peds,
                  std::vector<Target> const& /*unused*/, std::vector<bool>& met,
                  std::vector<bool> const& /*unused*/, BoolOp /*unused*/);

 private:
  VIPRA::idx              _event{};
  Behaviors::NumericValue _requiredTime;
  VIPRA::time_s           _startTime{};

  // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
  static inline constexpr auto in_time_step(VIPRA::time_s  currTime,
                                            VIPRA::time_s  checkTime,
                                            VIPRA::delta_t deltaT) -> bool
  {
    const VIPRA::delta_t left = checkTime - deltaT;
    const VIPRA::delta_t right = checkTime + deltaT;
    return (currTime > left && currTime < right);
  }
};
}  // namespace VIPRA::Behaviors