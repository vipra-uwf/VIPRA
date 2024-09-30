#pragma once

#include "vipra/vipra_behaviors/definitions/behavior_context.hpp"
#include "vipra/vipra_behaviors/definitions/sim_pack.hpp"
#include "vipra/vipra_behaviors/events/event.hpp"
#include "vipra/vipra_behaviors/targets/target.hpp"

namespace VIPRA::Behaviors {
class SubConditionStart {
  DEFAULT_CONSTRUCTIBLE(SubConditionStart)
  COPYABLE(SubConditionStart)
  MOVEABLE(SubConditionStart)

 public:
  void operator()(auto /*unused*/, VIPRA::idxVec const& /*unused*/,
                  std::vector<Target> const& /*unused*/, std::vector<bool>& met,
                  std::vector<bool> const& /*unused*/, BoolOp /*unused*/) const
  {
    std::fill(met.begin(), met.end(), true);
  }
};
}  // namespace VIPRA::Behaviors