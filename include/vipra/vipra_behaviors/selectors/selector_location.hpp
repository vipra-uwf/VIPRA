
#ifndef VIPRA_SELECTOR_LOCATION_HPP
#define VIPRA_SELECTOR_LOCATION_HPP

#include "selectors/selector.hpp"
#include "values/numeric_value.hpp"

namespace BHVR {
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
}  // namespace BHVR

#endif  //VIPRA_EXACTLY_N_RANDOM_SELECTOR_H
