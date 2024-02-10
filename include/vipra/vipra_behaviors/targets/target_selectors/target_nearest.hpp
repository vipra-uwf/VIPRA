#pragma once

#include <limits>
#include <optional>

#include "definitions/pedestrian_types.hpp"
#include "definitions/sim_pack.hpp"
#include "targets/target.hpp"
#include "targets/target_modifier.hpp"
#include "vipra/geometry/f3d.hpp"

namespace VIPRA::Behaviors {
/**
  * @brief Selects the nearest pedestrian with any of the given types
  * 
  */
struct TargetNearest {
  NON_DEFAULT_CONSTRUCTIBLE(TargetNearest)
  COPYABLE(TargetNearest)
  MOVEABLE(TargetNearest)

  Ptype                         type;
  bool                          allPeds;
  std::optional<TargetModifier> modifier;

  /**
   * @brief Returns the nearest pedestrian that has any of the target types
   * 
   * @param pack : simulation pack
   * @param self : pedestrain calling the function
   * @return Target 
   */
  inline auto operator()(Simpack pack, Self self) const -> Target {
    if (allPeds) {
      auto curr = nearest_in_group(pack, self.target.targetIdx, pack.get_groups().get_group(0));
      if (curr.second == VIPRA::idx_INVALID) return Target{TargetType::INVALID, 0};
      return {TargetType::PEDESTRIAN, curr.second};
    }

    VIPRA::dist shortest = std::numeric_limits<VIPRA::dist>::max();
    VIPRA::idx  nearest = VIPRA::idx_INVALID;

    type.for_each_type([&](typeUID type) {
      VIPRA::idx groupIdx = GroupsContainer::index(type);
      auto       curr = nearest_in_group(pack, self.target.targetIdx, pack.get_groups().get_group(groupIdx));
      if (curr.first < shortest) {
        shortest = curr.first;
        nearest = curr.second;
      }
    });

    if (nearest == VIPRA::idx_INVALID) {
      return Target{TargetType::INVALID, 0};
    }

    return Target{TargetType::PEDESTRIAN, nearest};
  }

 private:
  /**
   * @brief Returns the nearest pedestrian and the distance to them in a given group
   * 
   * @param pack : simulation pack
   * @param self : pedestrian looking for target
   * @param idxs : vector of pedestrian indexes in group
   * @return std::pair<VIPRA::dist, VIPRA::idx> : nearest distance and nearest pedestrian index
   */
  [[nodiscard]] inline auto nearest_in_group(Simpack pack, VIPRA::idx self, const VIPRA::idxVec& idxs) const
      -> std::pair<VIPRA::dist, VIPRA::idx> {
    VIPRA::dist shortest = std::numeric_limits<VIPRA::dist>::max();
    VIPRA::idx  nearest = VIPRA::idx_INVALID;

    auto const&      coords = pack.get_pedset().getCoordinates();
    const VIPRA::f3d currCoords = coords[self];

    for (VIPRA::idx pedIdx : idxs) {
      if (pedIdx == self) continue;

      if (modifier) {
        if (!modifier->check(pack, pedIdx, self)) continue;
      }

      if (pack.get_obsset().rayHit(currCoords, coords[pedIdx]) != -1) continue;

      VIPRA::dist curr = currCoords.distanceTo(coords[pedIdx]);
      if (curr < shortest) {
        shortest = curr;
        nearest = pedIdx;
      }
    }
    return {shortest, nearest};
  }
};
}  // namespace VIPRA::Behaviors
