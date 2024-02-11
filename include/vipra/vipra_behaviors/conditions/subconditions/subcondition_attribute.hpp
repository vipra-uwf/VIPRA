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
      : _type(type), _value(val), _not(negative) {}

  void operator()(auto pack, const VIPRA::idxVec& peds, std::vector<Target> const& targets,
                  std::vector<bool>& met, std::vector<bool> const& /*unused*/, BoolOp /*unused*/) const {
    for (auto ped : peds) {
      auto attr = AttributeHandling::get_value(targets[ped], _type, pack);

      if (_value.type == Type::TOWARDS_LOC || _value.type == Type::TOWARDS_ATTR) {
        met[ped] = towards_compare(attr, pack, targets[ped].targetIdx);
      }

      if (_not) {
        met[ped] = AttributeHandling::is_not_equal(attr, _value, pack);
      }

      met[ped] = AttributeHandling::is_equal(attr, _value, pack);
    }
  }

  [[nodiscard]] auto individual(auto pack, VIPRA::idx self, Target target) const -> bool {
    auto attr = AttributeHandling::get_value(target, _type, pack);

    if (_value.type == Type::TOWARDS_LOC || _value.type == Type::TOWARDS_ATTR) {
      return towards_compare(attr, pack, self);
    }

    if (_not) {
      return AttributeHandling::is_not_equal(attr, _value, pack);
    }

    return AttributeHandling::is_equal(attr, _value, pack);
  }

 private:
  Attribute       _type;
  CAttributeValue _value;
  bool            _not;

  static constexpr VIPRA::f_pnt TOWARDS_THRESHOLD = 0.5F;

  [[nodiscard]] inline auto towards_compare(CAttributeValue& attr, auto pack, VIPRA::idx self) const -> bool {
    if (_value.type == Type::TOWARDS_LOC) {
      return towards_location_compare(attr, pack, self);
    }

    return towards_attribute_compare(attr, pack, self);
  }

  [[nodiscard]] inline auto towards_location_compare(CAttributeValue& attr, auto pack, VIPRA::idx self) const
      -> bool {
    attr.type_check(Type::COORD);
    const VIPRA::f3d selfPos = pack.get_pedset().getPedCoords(self);
    auto const&      loc = pack.get_context().locations.at(_value.as<VIPRA::idx>());

    VIPRA::f3d checkDiff;
    VIPRA::f3d endDiff;

    switch (_type) {
      case Attribute::DIMENSIONS:
      case Attribute::STATE:
      case Attribute::LOCATION:
      case Attribute::STATUS:
      case Attribute::POSITION:
      case Attribute::INVALID:
        // TODO(rolland): build time check for this
        throw std::runtime_error(
            "Towards Location Compare not implemented for this "
            "attribute");
      case Attribute::END_GOAL:
        [[fallthrough]];
      case Attribute::CURR_GOAL:
        if (loc.contains(attr.as<VIPRA::f3d>())) return true;
        checkDiff = attr.as<VIPRA::f3d>() - selfPos;
        endDiff = loc.center() - selfPos;
      case Attribute::VELOCITY:
        checkDiff = attr.as<VIPRA::f3d>();
        endDiff = loc.center() - selfPos;
        break;
    }

    return endDiff.dot(checkDiff) > TOWARDS_THRESHOLD;
  }

  [[nodiscard]] inline auto towards_attribute_compare(CAttributeValue&, auto, VIPRA::idx) const -> bool {
    // TODO(rolland): implement this
    throw std::runtime_error("Towards Attribute Not implemented");
    return false;
  }
};
}  // namespace VIPRA::Behaviors
