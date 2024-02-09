#ifndef VIPRA_BEHAVIOR_CONDITION_HPP
#define VIPRA_BEHAVIOR_CONDITION_HPP

#include <memory>
#include <optional>
#include <vector>

#include "goals/goals.hpp"
#include "obstacle_set/obstacle_set.hpp"
#include "pedestrian_set/pedestrian_set.hpp"

#include "conditions/sub_condition.hpp"
#include "definitions/sim_pack.hpp"
#include "targets/target.hpp"
#include "util/class_types.hpp"
#include "util/timed_latch.hpp"
#include "vipra/geometry/f3d.hpp"

namespace BHVR {
class Condition {
  DEFAULT_CONSTRUCTIBLE(Condition)
  COPYABLE(Condition)
  MOVEABLE(Condition)
 public:
  void initialize(Simpack const&);

  void evaluate(Simpack, const VIPRA::idxVec&, std::vector<bool>&, std::vector<Target> const&,
                std::optional<TimedLatchCollection>&);

  void add_operation(BoolOp oper) { _steps.emplace_back(oper); }
  void add_subcondition(SubCondition&& condition) { _conditions.emplace_back(condition); }

 private:
  std::vector<SubCondition> _conditions;
  std::vector<BoolOp>       _steps;
  std::vector<bool>         _temp;
};
}  // namespace BHVR

#endif