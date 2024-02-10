#ifndef EVERYONE_SELECTOR_HPP
#define EVERYONE_SELECTOR_HPP

#include <selectors/selector.hpp>

namespace VIPRA::Behaviors {
/**
  * @brief Selects every pedestrian
  * 
  */
struct SelectorEveryone {
  DEFAULT_CONSTRUCTIBLE(SelectorEveryone)
  COPYABLE(SelectorEveryone)
  MOVEABLE(SelectorEveryone)

  auto operator()(const VIPRA::idxVec&, const VIPRA::idxVec&, Simpack) -> SelectorResult;
};

}  // namespace VIPRA::Behaviors

#endif