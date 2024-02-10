#pragma once

#include "vipra/vipra_behaviors/conditions/sub_condition.hpp"
#include "vipra/vipra_behaviors/definitions/sim_pack.hpp"
#include "vipra/vipra_behaviors/util/class_types.hpp"

namespace VIPRA::Behaviors {
class SubConditionLeave {
  NON_DEFAULT_CONSTRUCTIBLE(SubConditionLeave)
  COPYABLE(SubConditionLeave)
  MOVEABLE(SubConditionLeave)

 public:
  explicit SubConditionLeave(VIPRA::idx location) : _location{location} {}

  void operator()(auto pack, const VIPRA::idxVec&, std::vector<Target> const&, std::vector<bool>&,
                  std::vector<bool> const&, BoolOp);

 private:
  VIPRA::idx _location;

  // TODO(rolland): temporary fix for not having pedestrian memory
  std::vector<bool> _left;
};
}  // namespace VIPRA::Behaviors
