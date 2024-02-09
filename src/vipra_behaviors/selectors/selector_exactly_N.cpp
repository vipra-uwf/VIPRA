
#include <algorithm>

#include "randomization/random.hpp"
#include "vipra/geometry/f3d.hpp"


#include "selectors/selector_exactly_N.hpp"

namespace BHVR {
/**
   * @brief Selects an exact number of pedestrians from a group
   * 
   * @param rngEngine : randomization engine
   * @param group : group to pull from
   * @return SelectorResult 
   */
auto SelectorExactlyN::operator()(const VIPRA::idxVec& /*unused*/, const VIPRA::idxVec& group,
                                  Simpack pack) const -> SelectorResult {
  auto groupPeds = group;

  auto pedCnt = static_cast<VIPRA::size>(std::round(selectCount.value(0)));

  bool starved = false;
  if (pedCnt > group.size()) {
    starved = true;
    pedCnt = group.size();
  }

  // spdlog::debug("Selector Exaclty N: Selecting {} Pedestrians", pedCnt);

  std::shuffle(groupPeds.begin(), groupPeds.end(), pack.get_context().engine);
  groupPeds.resize(pedCnt);

  return {starved, groupPeds};
}
}  // namespace BHVR