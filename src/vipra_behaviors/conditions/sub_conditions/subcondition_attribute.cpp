

#include <attributes/attributes.hpp>
#include <conditions/subconditions/subcondition_attribute.hpp>
#include <definitions/dimensions.hpp>
#include "conditions/sub_condition.hpp"

namespace VIPRA::Behaviors {
SubConditionAttribute::SubConditionAttribute(Attribute type, CAttributeValue val, bool negative)
    : _type(type), _value(val), _not(negative) {}

/**
 * @brief Returns true if the targets attribute equals the value, or does not equal the value if the subcondition is negative.
 * 
 */
void SubConditionAttribute::operator()(Simpack pack, const VIPRA::idxVec& peds,
                                       std::vector<Target> const& targets, std::vector<bool>& met,
                                       std::vector<bool> const& /*unused*/, BoolOp /*unused*/) const {
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

auto SubConditionAttribute::individual(Simpack pack, VIPRA::idx self, Target target) const -> bool {
  auto attr = AttributeHandling::get_value(target, _type, pack);

  if (_value.type == Type::TOWARDS_LOC || _value.type == Type::TOWARDS_ATTR) {
    return towards_compare(attr, pack, self);
  }

  if (_not) {
    return AttributeHandling::is_not_equal(attr, _value, pack);
  }

  return AttributeHandling::is_equal(attr, _value, pack);
}

auto SubConditionAttribute::towards_compare(CAttributeValue& attr, Simpack pack, VIPRA::idx self) const
    -> bool {
  if (_value.type == Type::TOWARDS_LOC) {
    return towards_location_compare(attr, pack, self);
  }

  return towards_attribute_compare(attr, pack, self);
}

inline auto SubConditionAttribute::towards_location_compare(CAttributeValue& attr, Simpack pack,
                                                            VIPRA::idx self) const -> bool {
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
      // TODO: build time check for this
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

inline auto SubConditionAttribute::towards_attribute_compare(CAttributeValue&, Simpack, VIPRA::idx) const
    -> bool {
  throw std::runtime_error("Towards Attribute Not implemented");
  return false;
}

}  // namespace VIPRA::Behaviors