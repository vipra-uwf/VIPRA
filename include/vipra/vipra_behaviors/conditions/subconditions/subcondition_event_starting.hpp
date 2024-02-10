#pragma once

#include "vipra/vipra_behaviors/conditions/sub_condition.hpp"
#include "vipra/vipra_behaviors/definitions/sim_pack.hpp"
#include "vipra/vipra_behaviors/events/event.hpp"
#include "vipra/vipra_behaviors/util/class_types.hpp"

namespace VIPRA::Behaviors {
class SubConditionEventStarting {
  NON_DEFAULT_CONSTRUCTIBLE(SubConditionEventStarting)
  COPYABLE(SubConditionEventStarting)
  MOVEABLE(SubConditionEventStarting)
 public:
  explicit SubConditionEventStarting(VIPRA::idx event) : _event(event) {}

  void operator()(auto pack, const VIPRA::idxVec&, std::vector<Target> const&, std::vector<bool>&,
                  std::vector<bool> const&, BoolOp) const;

 private:
  VIPRA::idx _event;
};
}  // namespace VIPRA::Behaviors
