#ifndef VIPRA_SUBCONDITION_ATTRIBUTE_HPP
#define VIPRA_SUBCONDITION_ATTRIBUTE_HPP

#include "attributes/attributes.hpp"
#include "conditions/sub_condition.hpp"
#include "util/class_types.hpp"

namespace BHVR {
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

  void operator()(Simpack, const VIPRA::idxVec&, const std::vector<Target>&, std::vector<bool>&,
                  const std::vector<bool>&, BoolOp) const;

  [[nodiscard]] auto individual(Simpack, VIPRA::idx, Target) const -> bool;

 private:
  Attribute       _type;
  CAttributeValue _value;
  bool            _not;

  static constexpr float TOWARDS_THRESHOLD = 0.5F;

  [[nodiscard]] inline auto towards_compare(CAttributeValue&, Simpack, VIPRA::idx) const -> bool;
  [[nodiscard]] inline auto towards_location_compare(CAttributeValue&, Simpack, VIPRA::idx) const -> bool;
  [[nodiscard]] inline auto towards_attribute_compare(CAttributeValue&, Simpack, VIPRA::idx) const -> bool;
};
}  // namespace BHVR

#endif