#pragma once

#include "vipra/geometry/f3d.hpp"

#include "vipra/vipra_behaviors/conditions/sub_condition.hpp"
#include "vipra/vipra_behaviors/definitions/dsl_types.hpp"
#include "vipra/vipra_behaviors/events/event.hpp"
#include "vipra/vipra_behaviors/time/time.hpp"
#include "vipra/vipra_behaviors/util/class_types.hpp"
#include "vipra/vipra_behaviors/util/timed_latch.hpp"
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
      : _event(event), _requiredTime(std::move(time)) {}

  void operator()(auto pack, const VIPRA::idxVec&, std::vector<Target> const&, std::vector<bool>&,
                  std::vector<bool> const&, BoolOp);

 private:
  VIPRA::idx              _event{};
  Behaviors::NumericValue _requiredTime{};
  VIPRA::time_s           _startTime{};
};
}  // namespace VIPRA::Behaviors