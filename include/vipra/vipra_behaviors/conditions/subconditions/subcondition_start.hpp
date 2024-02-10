#pragma once

#include "definitions/behavior_context.hpp"
#include "definitions/sim_pack.hpp"
#include "events/event.hpp"
#include "targets/target.hpp"

namespace VIPRA::Behaviors {
class SubConditionStart {
  DEFAULT_CONSTRUCTIBLE(SubConditionStart)
  COPYABLE(SubConditionStart)
  MOVEABLE(SubConditionStart)

 public:
  void operator()(Simpack /*unused*/, const VIPRA::idxVec& /*unused*/, std::vector<Target> const& /*unused*/,
                  std::vector<bool>& met, std::vector<bool> const& /*unused*/, BoolOp /*unused*/) const {
    std::fill(met.begin(), met.end(), true);
  }
};
}  // namespace VIPRA::Behaviors