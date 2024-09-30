#pragma once

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

  auto operator()(const VIPRA::idxVec& /*unused*/, const VIPRA::idxVec& group,
                  auto /*unused*/) -> SelectorResult
  {
    auto vec = VIPRA::idxVec(group.size());
    std::iota(vec.begin(), vec.end(), 0);
    return {false, vec};
  }
};

}  // namespace VIPRA::Behaviors