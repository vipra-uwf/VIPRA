#pragma once

#include <utility>

#include "definitions/sim_pack.hpp"
#include "values/numeric_value.hpp"

#include "util/class_types.hpp"

namespace VIPRA::Behaviors {
/**
  * @brief Target modifier for location
  * 
  */
class ModifierLocation {
  NON_DEFAULT_CONSTRUCTIBLE(ModifierLocation)
  COPYABLE(ModifierLocation)
  MOVEABLE(ModifierLocation)

 public:
  explicit ModifierLocation(VIPRA::idx location) : _location(location) {}

  /**
   * @brief Returns if target is within the location
   * 
   * @param pack : simulation pack
   * @param targetIdx : target to check
   * @param selfIdx : pedsetrian looking for target
   * @return true : if inside the location
   * @return false : if NOT inside the location
   */
  auto operator()(Simpack pack, VIPRA::idx targetIdx, VIPRA::idx selfIdx) const -> bool {
    auto const targetPos = pack.get_pedset().getPedCoords(targetIdx);
    return pack.get_context().locations[_location].contains(targetPos);
  }

 private:
  VIPRA::idx _location;
};
}  // namespace VIPRA::Behaviors
