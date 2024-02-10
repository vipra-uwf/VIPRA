
#pragma once

#include <selectors/selector.hpp>
#include <values/numeric_value.hpp>

namespace VIPRA::Behaviors {
/**
 * @brief Selects an exact number of pedestrians
 * 
 */
struct SelectorExactlyN {
  NON_DEFAULT_CONSTRUCTIBLE(SelectorExactlyN)
  COPYABLE(SelectorExactlyN)
  MOVEABLE(SelectorExactlyN)

  NumericValue selectCount;
  auto         operator()(const VIPRA::idxVec&, const VIPRA::idxVec&, Simpack) const -> SelectorResult;
};
}  // namespace VIPRA::Behaviors
