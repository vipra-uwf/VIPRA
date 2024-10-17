#pragma once

#include "vipra/vipra_behaviors/conditions/sub_condition.hpp"
#include "vipra/vipra_behaviors/conditions/subconditions/subcondition_attribute.hpp"
#include "vipra/vipra_behaviors/targets/target_modifier.hpp"
#include "vipra/vipra_behaviors/util/class_types.hpp"

namespace VIPRA::Behaviors {
/**
 * @brief SubCondition for target attribute to equal a value
 * 
 */
class SubConditionExists {
  NON_DEFAULT_CONSTRUCTIBLE(SubConditionExists)
  COPYABLE(SubConditionExists)
  MOVEABLE(SubConditionExists)

 public:
  explicit SubConditionExists(TargetModifier        modifier,
                              SubConditionAttribute condition)
      : _modifier(std::move(modifier)), _attributeCond(condition)
  {
  }

  void operator()(Simpack pack, const VIPRA::idxVec& peds,
                  std::vector<Target> const& /*unused*/, std::vector<bool>& met,
                  std::vector<bool> const& prevMet, BoolOp oper) const;

 private:
  // TODO(rolland): add in group type, currently just checks pedestrians
  TargetModifier        _modifier;
  SubConditionAttribute _attributeCond;

  size_t _count = 0;
};
}  // namespace VIPRA::Behaviors