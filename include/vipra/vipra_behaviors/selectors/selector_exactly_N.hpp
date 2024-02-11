
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
  auto         operator()(const VIPRA::idxVec&, const VIPRA::idxVec&, auto) const -> SelectorResult;
};
}  // namespace VIPRA::Behaviors
