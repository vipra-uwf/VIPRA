
#pragma once

#include "vipra/vipra_behaviors/selectors/selector.hpp"
#include "vipra/vipra_behaviors/selectors/subselector.hpp"
#include "vipra/vipra_behaviors/values/numeric_value.hpp"

namespace VIPRA::Behaviors {
/**
 * @brief Selects an exact number of pedestrians
 * 
 */
struct SelectorExactlyN {
  NON_DEFAULT_CONSTRUCTIBLE(SelectorExactlyN)
  COPYABLE(SelectorExactlyN)
  MOVEABLE(SelectorExactlyN)

  explicit SelectorExactlyN(NumericValue count) : selectCount(std::move(count)) {}

  NumericValue selectCount;
  auto         operator()(const VIPRA::idxVec& /*unused*/, const VIPRA::idxVec& group, auto pack) const
      -> SelectorResult {
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
};
}  // namespace VIPRA::Behaviors
