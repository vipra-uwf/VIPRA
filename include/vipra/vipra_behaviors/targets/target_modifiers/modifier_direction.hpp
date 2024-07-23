#pragma once

#include "vipra/vipra_behaviors/definitions/sim_pack.hpp"
#include "vipra/vipra_behaviors/values/direction.hpp"
#include "vipra/vipra_behaviors/values/numeric_value.hpp"

namespace VIPRA::Behaviors {
/**
 * @brief Target Modifier for direction to target
 * 
 */
class ModifierDirection {
  NON_DEFAULT_CONSTRUCTIBLE(ModifierDirection)
  COPYABLE(ModifierDirection)
  MOVEABLE(ModifierDirection)

 public:
  explicit ModifierDirection(Direction direction) : _direction(direction) {}

  /**
   * @brief Returns true if the target is in the desired direction
   * 
   * @param pack : simulation pack
   * @param targetIdx : target to check
   * @param selfIdx : pedestrain looking for target
   * @return true : if in direction
   * @return false : if not in direction
   */
  auto operator()(auto pack, VIPRA::idx targetIdx, VIPRA::idx selfIdx) const -> bool
  {
    auto const& goalCoord = pack.goals.current_goal(selfIdx);
    auto const& selfCoord = pack.pedset.ped_coords(selfIdx);
    auto const& targetCoord = pack.pedset.ped_coords(targetIdx);

    auto forward = goalCoord - selfCoord;
    auto dif = targetCoord - selfCoord;

    if ( _direction == Direction::FRONT ) {
      return dif.dot(forward) > RANGE;
    }

    return dif.dot(forward) <= RANGE;
  }

 private:
  Direction _direction;

  static constexpr VIPRA::f_pnt RANGE = 0.1F;
};
}  // namespace VIPRA::Behaviors
