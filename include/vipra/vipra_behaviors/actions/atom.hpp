#pragma once

#include <functional>

#include "vipra/vipra_behaviors/definitions/sim_pack.hpp"

#include "vipra/vipra_behaviors/targets/target.hpp"

namespace VIPRA::Behaviors {
using Atom =
    std::function<void(Simpack&, VIPRA::idxVec const&, std::vector<bool> const&,
                       std::vector<Target> const&)>;
}  // namespace VIPRA::Behaviors
