#pragma once

#include "vipra/vipra_behaviors/targets/target.hpp"
#include "vipra/vipra_behaviors/util/class_types.hpp"

namespace VIPRA::Behaviors {

/**
 * @brief Returns the target that calls this
 * 
 */
struct TargetSelf {
  DEFAULT_CONSTRUCTIBLE(TargetSelf)
  COPYABLE(TargetSelf)
  MOVEABLE(TargetSelf)

  inline auto operator()(auto /*unused*/, Self self) -> Target { return self.target; }
};
}  // namespace VIPRA::Behaviors
