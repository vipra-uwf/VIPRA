#pragma once

#include <limits>
#include <optional>

#include "vipra/geometry/f3d.hpp"

#include "vipra/vipra_behaviors/definitions/pedestrian_types.hpp"
#include "vipra/vipra_behaviors/definitions/sim_pack.hpp"
#include "vipra/vipra_behaviors/targets/target.hpp"
#include "vipra/vipra_behaviors/targets/target_modifier.hpp"

namespace VIPRA::Behaviors {
/**
  * @brief Selects the nearest pedestrian with any of the given types
  * 
  */
template <typename modifier_t>
struct TargetNearest {
  NON_DEFAULT_CONSTRUCTIBLE(TargetNearest)
  COPYABLE(TargetNearest)
  MOVEABLE(TargetNearest)

  Ptype                     type;
  bool                      allPeds;
  std::optional<modifier_t> modifier;

  explicit TargetNearest(Ptype type, bool allPeds = false, std::optional<modifier_t> modifier = std::nullopt)
      : type(type), allPeds(allPeds), modifier(modifier) {}

  /**
   * @brief Returns the nearest pedestrian that has any of the target types
   * 
   * @param pack : simulation pack
   * @param self : pedestrain calling the function
   * @return Target 
   */
  inline auto operator()(auto pack, Self self) const -> Target {
    if (allPeds) {
      auto curr = nearest_in_group(pack, self.target.targetIdx, pack.groups.get_group(0));
      if (curr.second == VIPRA::INVALID_IDX) return Target{TargetType::INVALID, 0};
      return {TargetType::PEDESTRIAN, curr.second};
    }

    VIPRA::f_pnt shortest = std::numeric_limits<VIPRA::f_pnt>::max();
    VIPRA::idx   nearest = VIPRA::INVALID_IDX;

    type.for_each_type([&](typeUID type) {
      VIPRA::idx groupIdx = GroupsContainer::index(type);
      auto       curr = nearest_in_group(pack, self.target.targetIdx, pack.groups.get_group(groupIdx));
      if (curr.first < shortest) {
        shortest = curr.first;
        nearest = curr.second;
      }
    });

    if (nearest == VIPRA::INVALID_IDX) {
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
  [[nodiscard]] inline auto nearest_in_group(auto pack, VIPRA::idx self, VIPRA::idxVec const& idxs) const
      -> std::pair<VIPRA::f_pnt, VIPRA::idx> {
    VIPRA::f_pnt shortest = std::numeric_limits<VIPRA::f_pnt>::max();
    VIPRA::idx   nearest = VIPRA::INVALID_IDX;

    auto const&      coords = pack.pedset.all_coords();
    const VIPRA::f3d currCoords = coords[self];

    for (VIPRA::idx pedIdx : idxs) {
      if (pedIdx == self) continue;

      if (modifier) {
        if (!modifier->check(pack, pedIdx, self)) continue;
      }

      if (pack.obsset.ray_hit(currCoords, coords[pedIdx]) != -1) continue;

      VIPRA::f_pnt curr = currCoords.distance_to(coords[pedIdx]);
      if (curr < shortest) {
        shortest = curr;
        nearest = pedIdx;
      }
    }
    return {shortest, nearest};
  }
};
}  // namespace VIPRA::Behaviors
