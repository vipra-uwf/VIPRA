#pragma once

#include "vipra/geometry/f3d.hpp"

#include "vipra/vipra_behaviors/conditions/sub_condition.hpp"
#include "vipra/vipra_behaviors/definitions/dsl_types.hpp"
#include "vipra/vipra_behaviors/definitions/sim_pack.hpp"
#include "vipra/vipra_behaviors/events/event.hpp"
#include "vipra/vipra_behaviors/time/time.hpp"
#include "vipra/vipra_behaviors/util/class_types.hpp"
#include "vipra/vipra_behaviors/util/timed_latch.hpp"
#include "vipra/vipra_behaviors/values/numeric_value.hpp"

namespace VIPRA::Behaviors {
class SubConditionEnter {
  NON_DEFAULT_CONSTRUCTIBLE(SubConditionEnter)
  COPYABLE(SubConditionEnter)
  MOVEABLE(SubConditionEnter)

 public:
  explicit SubConditionEnter(VIPRA::idx location) : _location{location} {}

  void operator()(auto pack, const VIPRA::idxVec&, std::vector<Target> const&, std::vector<bool>&,
                  std::vector<bool> const&, BoolOp);

 private:
  VIPRA::idx _location;

  // TODO(rolland): temporary fix for not having pedestrian memory
  std::vector<bool> _entered;
};
}  // namespace VIPRA::Behaviors