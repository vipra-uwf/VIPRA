#pragma once

#include "vipra/types/idx.hpp"
#include "vipra/vipra_behaviors/selectors/selector.hpp"
#include "vipra/vipra_behaviors/selectors/subselector.hpp"
#include "vipra/vipra_behaviors/values/numeric_value.hpp"

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
  auto       operator()(const VIPRA::idxVec&, const VIPRA::idxVec& group, auto pack) const -> SelectorResult {
    auto const&   loc = pack.get_context().locations[location];
    size_t        pedCnt = 0;
    VIPRA::idxVec groupPeds;

    for (auto idx : group) {
            if (loc.contains(pack.get_pedset().getPedCoords(idx))) {
              groupPeds.push_back(idx);
      }
    }

    // spdlog::debug("Selector Exaclty N: Selecting {} Pedestrians", pedCnt);

    return {false, groupPeds};
  }
};
}  // namespace VIPRA::Behaviors
