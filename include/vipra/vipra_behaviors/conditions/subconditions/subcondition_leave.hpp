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

  void operator()(Simpack pack, const VIPRA::idxVec& peds,
                  std::vector<Target> const& targets, std::vector<bool>& met,
                  std::vector<bool> const& /*unused*/, BoolOp /*unused*/);

 private:
  VIPRA::idx _location;

  // TODO(rolland): temporary fix for not having pedestrian memory
  std::vector<bool> _left;
};
}  // namespace VIPRA::Behaviors
