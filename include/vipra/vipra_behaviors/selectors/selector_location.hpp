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
  auto       operator()(const VIPRA::idxVec&, const VIPRA::idxVec&, auto) const -> SelectorResult;
};
}  // namespace VIPRA::Behaviors
