#pragma once

#include "vipra/logging/logging.hpp"
#include "vipra/types/idx.hpp"

#include "vipra/vipra_behaviors/selectors/subselector.hpp"

namespace VIPRA::Behaviors {
/**
 * @brief Selects an exact number of pedestrians
 * 
 */
struct SelectorLocation {
  NON_DEFAULT_CONSTRUCTIBLE(SelectorLocation)
  COPYABLE(SelectorLocation)
  MOVEABLE(SelectorLocation)

  explicit SelectorLocation(VIPRA::idx loc) : location(loc) {}

  VIPRA::idx location;
  auto operator()(const VIPRA::idxVec& /*unused*/, const VIPRA::idxVec& group,
                  auto pack) const -> SelectorResult
  {
    auto const&   loc = pack.context.locations[location];
    size_t        pedCnt = 0;
    VIPRA::idxVec groupPeds;

    for ( auto idx : group ) {
      if ( loc.contains(pack.pedset.ped_coords(idx)) ) {
        groupPeds.push_back(idx);
      }
    }

    VIPRA::Log::debug("Selector Exaclty N: Selecting {} Pedestrians", pedCnt);

    return {false, groupPeds};
  }
};
}  // namespace VIPRA::Behaviors
