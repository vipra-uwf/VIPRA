#pragma once

#include <limits>
#include <optional>
#include <utility>

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
struct TargetNearest {
  NON_DEFAULT_CONSTRUCTIBLE(TargetNearest)
  COPYABLE(TargetNearest)
  MOVEABLE(TargetNearest)

  Ptype                         type;
  bool                          allPeds;
  std::optional<TargetModifier> modifier;

  explicit TargetNearest(Ptype type, bool allPeds = false,
                         std::optional<TargetModifier> modifier = std::nullopt)
      : type(type), allPeds(allPeds), modifier(std::move(modifier))
  {
  }

  /**
   * @brief Returns the nearest pedestrian that has any of the target types
   * 
   * @param pack : simulation pack
   * @param self : pedestrain calling the function
   * @return Target 
   */
  inline auto operator()(Simpack pack, Self self) const -> Target
  {
    if ( allPeds ) {
      auto curr = nearest_in_group(pack, self.target.targetIdx,
                                   pack.groups.get_group(0));
      if ( curr.second == VIPRA::INVALID_IDX )
        return Target{TargetType::INVALID, 0};
      return {TargetType::PEDESTRIAN, curr.second};
    }

    VIPRA::f_pnt shortest = std::numeric_limits<VIPRA::f_pnt>::max();
    VIPRA::idx   nearest = VIPRA::INVALID_IDX;

    type.for_each_type([&](typeUID type) {
      VIPRA::idx groupIdx = GroupsContainer::index(type);
      auto       curr = nearest_in_group(pack, self.target.targetIdx,
                                         pack.groups.get_group(groupIdx));
      if ( curr.first < shortest ) {
        shortest = curr.first;
        nearest = curr.second;
      }
    });

    if ( nearest == VIPRA::INVALID_IDX ) {
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
  [[nodiscard]] inline auto nearest_in_group(Simpack pack, VIPRA::idx self,
                                             VIPRA::idxVec const& idxs) const
      -> std::pair<VIPRA::f_pnt, VIPRA::idx>
  {
    VIPRA::f_pnt shortest = std::numeric_limits<VIPRA::f_pnt>::max();
    VIPRA::idx   nearest = VIPRA::INVALID_IDX;

    auto const&      coords = pack.pedset.all_coords();
    const VIPRA::f3d currCoords = coords[self];

    nearest = pack.pedset.conditional_closest_ped(self, [&](VIPRA::idx other) {
      if ( std::find(idxs.begin(), idxs.end(), other) == idxs.end() )
        return false;

      if ( modifier ) {
        if ( ! modifier->check(pack, self, other) ) return false;
      }

      if ( pack.map.ray_hit(currCoords, coords[other]) != -1 ) return false;

      return true;
    });

    return {currCoords.distance_to(coords[nearest]), nearest};
  }
};
}  // namespace VIPRA::Behaviors
