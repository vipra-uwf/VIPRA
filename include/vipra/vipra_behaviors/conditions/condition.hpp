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
                std::optional<TimedLatchCollection>& latches);

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
                             std::vector<bool>&                   met);
};
}  // namespace VIPRA::Behaviors
