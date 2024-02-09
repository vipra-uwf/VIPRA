#ifndef ID_RATIO_SELECTOR_HPP
#define ID_RATIO_SELECTOR_HPP

#include <selectors/selector.hpp>

namespace BHVR {
/**
 * @brief Selects a percentage of a group to be a type
 * 
 */
struct SelectorPercent {
  NON_DEFAULT_CONSTRUCTIBLE(SelectorPercent)
  COPYABLE(SelectorPercent)
  MOVEABLE(SelectorPercent)

  float percentage;
  auto  operator()(const std::vector<VIPRA::idx>&, const std::vector<VIPRA::idx>&, Simpack) const
      -> SelectorResult;
};

}  // namespace BHVR

#endif