#pragma once

#include "vipra/vipra_behaviors/conditions/sub_condition.hpp"
#include "vipra/vipra_behaviors/definitions/behavior_context.hpp"
#include "vipra/vipra_behaviors/definitions/sim_pack.hpp"
#include "vipra/vipra_behaviors/events/event.hpp"
#include "vipra/vipra_behaviors/util/class_types.hpp"

namespace VIPRA::Behaviors {

/**
 * @brief SubCondition for target attribute to equal a value
 * 
 */
class SubConditionEventOccurred {
  NON_DEFAULT_CONSTRUCTIBLE(SubConditionEventOccurred)
  COPYABLE(SubConditionEventOccurred)
  MOVEABLE(SubConditionEventOccurred)
 public:
  explicit SubConditionEventOccurred(VIPRA::idx event, bool negate)
      : _event(event), _negate(negate)
  {
  }

  void operator()(Simpack pack, const VIPRA::idxVec& /*unused*/,
                  std::vector<Target> const& /*unused*/, std::vector<bool>& met,
                  std::vector<bool> const& /*unused*/, BoolOp /*unused*/) const
  {
    if ( _negate ) {
      std::fill(met.begin(), met.end(), ! pack.context.events[_event].has_occurred());
    }
    else {
      std::fill(met.begin(), met.end(), pack.context.events[_event].has_occurred());
    }
  }

 private:
  VIPRA::idx _event;
  bool       _negate{false};
};

}  // namespace VIPRA::Behaviors
