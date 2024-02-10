
#include "randomization/random.hpp"
#include "vipra/geometry/f3d.hpp"

#include "selectors/selector_location.hpp"

namespace VIPRA::Behaviors {
/**
   * @brief Selects an exact number of pedestrians from a group
   * 
   * @param rngEngine : randomization engine
   * @param group : group to pull from
   * @return SelectorResult 
   */
auto SelectorLocation::operator()(const VIPRA::idxVec& /*unused*/, const VIPRA::idxVec& group,
                                  Simpack pack) const -> SelectorResult {
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
}  // namespace VIPRA::Behaviors