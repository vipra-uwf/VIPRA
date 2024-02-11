#pragma once

#include "vipra/vipra_behaviors/conditions/sub_condition.hpp"
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

  void operator()(auto pack, const VIPRA::idxVec&, std::vector<Target> const&, std::vector<bool>&,
                  std::vector<bool> const&, BoolOp) const;

 private:
  VIPRA::idx _event;
};
}  // namespace VIPRA::Behaviors