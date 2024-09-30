#pragma once

#include "vipra/vipra_behaviors/conditions/sub_condition.hpp"
#include "vipra/vipra_behaviors/definitions/behavior_context.hpp"
#include "vipra/vipra_behaviors/definitions/sim_pack.hpp"
#include "vipra/vipra_behaviors/events/event.hpp"
#include "vipra/vipra_behaviors/util/class_types.hpp"

namespace VIPRA::Behaviors {

class SubConditionEventOccurring {
  NON_DEFAULT_CONSTRUCTIBLE(SubConditionEventOccurring)
  COPYABLE(SubConditionEventOccurring)
  MOVEABLE(SubConditionEventOccurring)
 public:
  explicit SubConditionEventOccurring(VIPRA::idx event) : _event(event) {}

  void operator()(Simpack pack, const VIPRA::idxVec& /*unused*/,
                  std::vector<Target> const& /*unused*/, std::vector<bool>& met,
                  std::vector<bool> const& /*unused*/, BoolOp /*unused*/) const
  {
    std::fill(met.begin(), met.end(),
              pack.context.events[_event].is_occurring());
  }

 private:
  VIPRA::idx _event;
};
}  // namespace VIPRA::Behaviors