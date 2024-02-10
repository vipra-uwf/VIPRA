#pragma once

#include <memory>
#include <optional>
#include <vector>

#include "vipra/geometry/f3d.hpp"

#include "vipra/vipra_behaviors/conditions/sub_condition.hpp"
#include "vipra/vipra_behaviors/definitions/sim_pack.hpp"
#include "vipra/vipra_behaviors/targets/target.hpp"
#include "vipra/vipra_behaviors/util/class_types.hpp"
#include "vipra/vipra_behaviors/util/timed_latch.hpp"

namespace VIPRA::Behaviors {
template <typename subcond_t>
class Condition {
  DEFAULT_CONSTRUCTIBLE(Condition)
  COPYABLE(Condition)
  MOVEABLE(Condition)
 public:
  void initialize(auto const&);

  void evaluate(auto, const VIPRA::idxVec&, std::vector<bool>&, std::vector<Target> const&,
                std::optional<TimedLatchCollection>&);

  void add_operation(BoolOp oper) { _steps.emplace_back(oper); }
  void add_subcondition(subcond_t&& condition) { _conditions.emplace_back(condition); }

 private:
  std::vector<subcond_t> _conditions;
  std::vector<BoolOp>    _steps;
  std::vector<bool>      _temp;
};
}  // namespace VIPRA::Behaviors
