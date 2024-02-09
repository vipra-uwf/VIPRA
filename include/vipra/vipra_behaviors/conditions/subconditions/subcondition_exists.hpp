#ifndef VIPRA_SUBCONDITION_EXISTS_HPP
#define VIPRA_SUBCONDITION_EXISTS_HPP

#include <optional>

#include "attributes/attributes.hpp"
#include "conditions/sub_condition.hpp"
#include "conditions/subconditions/subcondition_attribute.hpp"
#include "targets/target_modifier.hpp"
#include "util/class_types.hpp"

namespace BHVR {
/**
 * @brief SubCondition for target attribute to equal a value
 * 
 */
class SubConditionExists {
  NON_DEFAULT_CONSTRUCTIBLE(SubConditionExists)
  COPYABLE(SubConditionExists)
  MOVEABLE(SubConditionExists)

 public:
  explicit SubConditionExists(TargetModifier modifier, SubConditionAttribute condition)
      : _modifier(std::move(modifier)), _attributeCond(condition) {}

  void operator()(Simpack, const VIPRA::idxVec&, const std::vector<Target>&, std::vector<bool>&,
                  const std::vector<bool>&, BoolOp) const;

 private:
  // TODO: add in group type, currently just checks pedestrians
  TargetModifier        _modifier;
  SubConditionAttribute _attributeCond;

  size_t _count = 0;
};
}  // namespace BHVR

#endif