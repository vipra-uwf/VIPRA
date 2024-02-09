#ifndef VIPRA_BEHAVIORS_MODIFIER_DIRECTION_HPP
#define VIPRA_BEHAVIORS_MODIFIER_DIRECTION_HPP

#include "definitions/sim_pack.hpp"
#include "values/direction.hpp"
#include "values/numeric_value.hpp"

namespace BHVR {
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
  auto operator()(Simpack pack, VIPRA::idx targetIdx, VIPRA::idx selfIdx) const -> bool {
    const auto& goalCoord = pack.get_goals().getCurrentGoal(selfIdx);
    const auto& selfCoord = pack.get_pedset().getPedCoords(selfIdx);
    const auto& targetCoord = pack.get_pedset().getPedCoords(targetIdx);

    auto forward = goalCoord - selfCoord;
    auto dif = targetCoord - selfCoord;

    if (_direction == Direction::FRONT) {
      return dif.dot(forward) > RANGE;
    }

    return dif.dot(forward) <= RANGE;
  }

 private:
  Direction _direction;

  static constexpr float RANGE = 0.1F;
};
}  // namespace BHVR

#endif