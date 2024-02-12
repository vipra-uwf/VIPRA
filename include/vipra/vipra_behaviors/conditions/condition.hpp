#pragma once

#include <algorithm>
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
  template <typename pack_t>
  void initialize(pack_t pack) {
    if (_conditions.size() > 1) {
      _temp.resize(pack.pedset.num_pedestrians());
      std::fill(_temp.begin(), _temp.end(), false);
    }
  }

  template <typename pack_t>
  void evaluate(pack_t pack, VIPRA::idxVec const& peds, std::vector<bool>& met,
                std::vector<Target> const& targets, std::optional<TimedLatchCollection>& /*latches*/) {
    // TODO(rolland): add in latch checks
    std::fill(_temp.begin(), _temp.end(), false);
    _conditions[0](pack, peds, targets, met, _temp, BoolOp::OR);

    for (VIPRA::idx i = 1; i < _conditions.size(); ++i) {
      _conditions[i](pack, peds, targets, _temp, met, _steps[i - 1]);
      switch (_steps[i - 1]) {
        case BoolOp::AND:
          std::transform(met.begin(), met.end(), _temp.begin(), met.begin(), std::logical_and<>());
          break;
        case BoolOp::OR:
          std::transform(met.begin(), met.end(), _temp.begin(), met.begin(), std::logical_or<>());
          break;
      }
    }
  }

  void add_operation(BoolOp oper) { _steps.emplace_back(oper); }
  void add_subcondition(subcond_t&& condition) { _conditions.emplace_back(condition); }

 private:
  std::vector<subcond_t> _conditions;
  std::vector<BoolOp>    _steps;
  std::vector<bool>      _temp;
};
}  // namespace VIPRA::Behaviors
