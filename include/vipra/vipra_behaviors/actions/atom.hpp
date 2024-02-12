#pragma once

#include <functional>

#include "vipra/vipra_behaviors/definitions/behavior_context.hpp"
#include "vipra/vipra_behaviors/definitions/sim_pack.hpp"

#include "vipra/vipra_behaviors/targets/target.hpp"

namespace VIPRA::Behaviors {

template <typename simpack_t>
using Atom = std::function<void(simpack_t, VIPRA::idxVec const&, std::vector<bool> const&,
                                std::vector<Target> const&)>;
}  // namespace VIPRA::Behaviors
