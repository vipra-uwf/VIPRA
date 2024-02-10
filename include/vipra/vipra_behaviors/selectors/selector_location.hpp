#pragma once

#include "selectors/selector.hpp"
#include "values/numeric_value.hpp"

namespace VIPRA::Behaviors {
/**
 * @brief Selects an exact number of pedestrians
 * 
 */
struct SelectorLocation {
  NON_DEFAULT_CONSTRUCTIBLE(SelectorLocation)
  COPYABLE(SelectorLocation)
  MOVEABLE(SelectorLocation)

  VIPRA::idx location;
  auto       operator()(const VIPRA::idxVec&, const VIPRA::idxVec&, Simpack) const -> SelectorResult;
};
}  // namespace VIPRA::Behaviors
