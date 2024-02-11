#pragma once

#include "vipra/vipra_behaviors/selectors/selector.hpp"
#include "vipra/vipra_behaviors/selectors/subselector.hpp"

namespace VIPRA::Behaviors {
/**
 * @brief Selects a percentage of a group to be a type
 * 
 */
struct SelectorPercent {
  NON_DEFAULT_CONSTRUCTIBLE(SelectorPercent)
  COPYABLE(SelectorPercent)
  MOVEABLE(SelectorPercent)

  explicit SelectorPercent(VIPRA::f_pnt pnt) : percentage(pnt) {}

  VIPRA::f_pnt percentage;
  auto         operator()(std::vector<VIPRA::idx> const& fullGroup, std::vector<VIPRA::idx> const& group,
                  auto pack) const -> SelectorResult {
    auto groupPeds = group;

    auto count =
        static_cast<VIPRA::size>(std::floor(percentage * static_cast<VIPRA::f_pnt>(fullGroup.size())));

    bool starved = false;
    if (count > group.size()) {
              starved = true;
              count = group.size();
    }

    // spdlog::debug("Selector Percent: Selecting {} Pedestrians", count);

    std::shuffle(groupPeds.begin(), groupPeds.end(), pack.get_context().engine);
    groupPeds.resize(count);

    return {starved, groupPeds};
  }
};

}  // namespace VIPRA::Behaviors
