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
  explicit SubConditionAttribute(Attribute type, CAttributeValue val, bool negative)
      : _type(type), _value(val), _not(negative)
  {
  }

  void operator()(Simpack pack, const VIPRA::idxVec& peds,
                  std::vector<Target> const& targets, std::vector<bool>& met,
                  std::vector<bool> const& /*unused*/, BoolOp /*unused*/) const;

  [[nodiscard]] auto individual(Simpack pack, VIPRA::idx self,
                                Target target) const -> bool;

 private:
  Attribute       _type;
  CAttributeValue _value;
  bool            _not;

  static constexpr VIPRA::f_pnt TOWARDS_THRESHOLD = 0.5F;

  [[nodiscard]] inline auto towards_compare(CAttributeValue& attr, Simpack pack,
                                            VIPRA::idx self) const -> bool;

  [[nodiscard]] inline auto towards_location_compare(CAttributeValue& attr, Simpack pack,
                                                     VIPRA::idx self) const -> bool;

  [[nodiscard]] inline auto towards_attribute_compare(
      CAttributeValue& /*unused*/, auto /*unused*/, VIPRA::idx /*unused*/) const -> bool;
};
}  // namespace VIPRA::Behaviors
