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
  auto         operator()(std::vector<VIPRA::idx> const&, std::vector<VIPRA::idx> const&, auto) const
      -> SelectorResult;
};

}  // namespace VIPRA::Behaviors
