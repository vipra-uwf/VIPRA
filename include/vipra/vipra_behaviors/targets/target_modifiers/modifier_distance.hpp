#ifndef VIPRA_BEHAVIORS_MODIFIER_DISTANCE_HPP
#define VIPRA_BEHAVIORS_MODIFIER_DISTANCE_HPP

#include <utility>

#include "definitions/sim_pack.hpp"
#include "util/class_types.hpp"
#include "values/numeric_value.hpp"

namespace BHVR {
/**
  * @brief Target modifier for distance to target
  * 
  */
class ModifierDistance {
  NON_DEFAULT_CONSTRUCTIBLE(ModifierDistance)
  COPYABLE(ModifierDistance)
  MOVEABLE(ModifierDistance)

 public:
  explicit ModifierDistance(NumericValue value) : _dist(std::move(value)) {}

  /**
   * @brief Returns if target is within distance
   * 
   * @param pack : simulation pack
   * @param targetIdx : target to check
   * @param selfIdx : pedsetrian looking for target
   * @return true : if within distance
   * @return false : if NOT within distance
   */
  auto operator()(Simpack pack, VIPRA::idx targetIdx, VIPRA::idx selfIdx) const -> bool {
    return pack.get_pedset().getPedCoords(selfIdx).distanceTo(pack.get_pedset().getPedCoords(targetIdx)) <=
           _dist.value(selfIdx);
  }

 private:
  NumericValue _dist;
};
}  // namespace BHVR

#endif