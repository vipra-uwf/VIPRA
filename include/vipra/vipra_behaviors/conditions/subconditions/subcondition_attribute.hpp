#pragma once

#include "vipra/types/idx.hpp"

#include "vipra/vipra_behaviors/attributes/attributes.hpp"
#include "vipra/vipra_behaviors/conditions/sub_condition.hpp"
#include "vipra/vipra_behaviors/util/class_types.hpp"

namespace VIPRA::Behaviors {
/**
 * @brief SubCondition for target attribute to equal a value
 * 
 */
class SubConditionAttribute {
  NON_DEFAULT_CONSTRUCTIBLE(SubConditionAttribute)
  COPYABLE(SubConditionAttribute)
  MOVEABLE(SubConditionAttribute)

 public:
  explicit SubConditionAttribute(Attribute, CAttributeValue, bool);

  void operator()(auto, const VIPRA::idxVec&, std::vector<Target> const&, std::vector<bool>&,
                  std::vector<bool> const&, BoolOp) const;

  [[nodiscard]] auto individual(auto, VIPRA::idx, Target) const -> bool;

 private:
  Attribute       _type;
  CAttributeValue _value;
  bool            _not;

  static constexpr VIPRA::f_pnt TOWARDS_THRESHOLD = 0.5F;

  [[nodiscard]] inline auto towards_compare(CAttributeValue&, auto, VIPRA::idx) const -> bool;
  [[nodiscard]] inline auto towards_location_compare(CAttributeValue&, auto, VIPRA::idx) const -> bool;
  [[nodiscard]] inline auto towards_attribute_compare(CAttributeValue&, auto, VIPRA::idx) const -> bool;
};
}  // namespace VIPRA::Behaviors
