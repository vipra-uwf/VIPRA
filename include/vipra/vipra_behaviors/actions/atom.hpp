#ifndef VIPRA_ACTION_ATOM_HPP
#define VIPRA_ACTION_ATOM_HPP

#include <functional>

#include "definitions/behavior_context.hpp"
#include "definitions/sim_pack.hpp"

#include "goals/goals.hpp"
#include "obstacle_set/obstacle_set.hpp"
#include "pedestrian_set/pedestrian_set.hpp"

#include "targets/target.hpp"

namespace BHVR {

using Atom =
    std::function<void(Simpack, const VIPRA::idxVec&, const std::vector<bool>&, const std::vector<Target>&)>;
}  // namespace BHVR

#endif