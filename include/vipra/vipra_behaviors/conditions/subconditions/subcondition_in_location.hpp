#pragma once

#include "vipra/vipra_behaviors/attributes/attributes.hpp"
#include "vipra/vipra_behaviors/conditions/sub_condition.hpp"
#include "vipra/vipra_behaviors/util/class_types.hpp"

namespace VIPRA::Behaviors {
/**
 * @brief SubCondition for target being in a location
 * 
 */
class SubConditionInLocation {
  NON_DEFAULT_CONSTRUCTIBLE(SubConditionInLocation)
  COPYABLE(SubConditionInLocation)
  MOVEABLE(SubConditionInLocation)

 public:
  explicit SubConditionInLocation(VIPRA::idx location) : _location(location) {}

  void operator()(auto pack, const VIPRA::idxVec&, std::vector<Target> const&, std::vector<bool>&,
                  std::vector<bool> const&, BoolOp) const;

 private:
  VIPRA::idx _location;
};
}  // namespace VIPRA::Behaviors
