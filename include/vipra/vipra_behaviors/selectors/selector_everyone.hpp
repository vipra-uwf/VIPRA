#pragma once

#include "vipra/vipra_behaviors/selectors/selector.hpp"
#include "vipra/vipra_behaviors/selectors/subselector.hpp"

namespace VIPRA::Behaviors {
/**
  * @brief Selects every pedestrian
  * 
  */
struct SelectorEveryone {
  DEFAULT_CONSTRUCTIBLE(SelectorEveryone)
  COPYABLE(SelectorEveryone)
  MOVEABLE(SelectorEveryone)

  auto operator()(const VIPRA::idxVec&, const VIPRA::idxVec&, auto) -> SelectorResult;
};

}  // namespace VIPRA::Behaviors