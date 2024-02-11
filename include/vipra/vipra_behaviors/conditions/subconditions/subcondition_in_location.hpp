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

  void operator()(auto pack, const VIPRA::idxVec& peds, std::vector<Target> const& targets,
                  std::vector<bool>& met, std::vector<bool> const& /*unused*/, BoolOp /*unused*/) const {
    for (auto idx : peds) {
      auto const& coords = pack.get_pedset().getPedCoords(targets[idx].targetIdx);
      met[idx] = pack.get_context().locations[_location].contains(coords);
    }
  }

 private:
  VIPRA::idx _location;
};
}  // namespace VIPRA::Behaviors
