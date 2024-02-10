#pragma once

#include <optional>

#include "vipra/vipra_behaviors/attributes/attributes.hpp"
#include "vipra/vipra_behaviors/conditions/sub_condition.hpp"
#include "vipra/vipra_behaviors/conditions/subconditions/subcondition_attribute.hpp"
#include "vipra/vipra_behaviors/targets/target_modifier.hpp"
#include "vipra/vipra_behaviors/util/class_types.hpp"

namespace VIPRA::Behaviors {
/**
 * @brief SubCondition for target attribute to equal a value
 * 
 */
template <typename modifier_t>
class SubConditionExists {
  NON_DEFAULT_CONSTRUCTIBLE(SubConditionExists)
  COPYABLE(SubConditionExists)
  MOVEABLE(SubConditionExists)

 public:
  explicit SubConditionExists(modifier_t modifier, SubConditionAttribute condition)
      : _modifier(std::move(modifier)), _attributeCond(condition) {}

  void operator()(auto pack, const VIPRA::idxVec&, std::vector<Target> const&, std::vector<bool>&,
                  std::vector<bool> const&, BoolOp) const;

 private:
  // TODO(rolland): add in group type, currently just checks pedestrians
  modifier_t            _modifier;
  SubConditionAttribute _attributeCond;

  size_t _count = 0;
};
}  // namespace VIPRA::Behaviors