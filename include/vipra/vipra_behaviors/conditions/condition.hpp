#pragma once

#include <algorithm>
#include <optional>
#include <vector>

#include "vipra/vipra_behaviors/conditions/sub_condition.hpp"
#include "vipra/vipra_behaviors/definitions/sim_pack.hpp"
#include "vipra/vipra_behaviors/targets/target.hpp"
#include "vipra/vipra_behaviors/util/class_types.hpp"
#include "vipra/vipra_behaviors/util/timed_latch.hpp"

namespace VIPRA::Behaviors {
class Condition {
  DEFAULT_CONSTRUCTIBLE(Condition)
  COPYABLE(Condition)
  MOVEABLE(Condition)
 public:
  void initialize(Simpack pack)
  {
    if ( _conditions.size() > 1 ) {
      _temp.resize(pack.pedset.num_pedestrians());
      std::fill(_temp.begin(), _temp.end(), false);
    }
  }

  /**
   * @brief Evaluates whether a condition is met for a group of pedestrians, writes to 'met' vector
   * 
   * @tparam Simpack 
   * @param pack 
   * @param peds 
   * @param met - output vector
   * @param targets
   * @param latches 
   */
  void evaluate(Simpack pack, VIPRA::idxVec const& peds, std::vector<bool>& met,
                std::vector<Target> const&           targets,
                std::optional<TimedLatchCollection>& latches)
  {
    std::fill(_temp.begin(), _temp.end(), false);
    _conditions[0](pack, peds, targets, met, _temp, BoolOp::OR);

    for ( VIPRA::idx i = 1; i < _conditions.size(); ++i ) {
      _conditions[i](pack, peds, targets, _temp, met, _steps[i - 1]);
      switch ( _steps[i - 1] ) {
        case BoolOp::AND:
          std::transform(met.begin(), met.end(), _temp.begin(), met.begin(),
                         std::logical_and<>());
          break;
        case BoolOp::OR:
          std::transform(met.begin(), met.end(), _temp.begin(), met.begin(),
                         std::logical_or<>());
          break;
      }
    }

    handle_latches(pack, latches, met);
  }

  void add_operation(BoolOp oper) { _steps.emplace_back(oper); }
  void add_subcondition(SubCondition&& condition)
  {
    _conditions.emplace_back(condition);
  }

 private:
  std::vector<SubCondition> _conditions;
  std::vector<BoolOp>       _steps;
  std::vector<bool>         _temp;

  /**
   * @brief Updates action duration latches
   * 
   * @tparam Simpack 
   * @param pack 
   * @param latches 
   * @param met 
   */
  static void handle_latches(Simpack                              pack,
                             std::optional<TimedLatchCollection>& latches,
                             std::vector<bool>&                   met)
  {
    if ( latches.has_value() ) {
      for ( VIPRA::idx i = 0; i < met.size(); ++i ) {
        if ( met[i] ) {
          latches->latch(pack.dT, i);
        }

        met[i] = latches->check(pack.dT, i);
      }
    }
  }
};
}  // namespace VIPRA::Behaviors
