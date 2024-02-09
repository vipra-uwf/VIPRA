
#ifndef VIPRA_EXACTLY_N_RANDOM_SELECTOR_H
#define VIPRA_EXACTLY_N_RANDOM_SELECTOR_H

#include <selectors/selector.hpp>
#include <values/numeric_value.hpp>

namespace BHVR {
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
}  // namespace BHVR

#endif  //VIPRA_EXACTLY_N_RANDOM_SELECTOR_H
