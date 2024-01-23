#pragma once

#include <cstdio>

#include "vipra/concepts/goals.hpp"
#include "vipra/concepts/parameters.hpp"

#include "vipra/modules.hpp"
#include "vipra/types/parameter.hpp"

namespace VIPRA::Goals {
class AStar {
  VIPRA_MODULE_TYPE(GOALS)
 public:
  template <Concepts::ParamModule params_t>
  static void register_params() {
    params_t::register_param(MODULE_TYPE, "endGoalType", VIPRA::Parameter{Parameter::Type::REQUIRED});
    params_t::register_param(MODULE_TYPE, "goalRange", VIPRA::Parameter{Parameter::Type::REQUIRED});
    params_t::register_param(MODULE_TYPE, "gridSize", VIPRA::Parameter{Parameter::Type::REQUIRED});
    params_t::register_param(MODULE_TYPE, "closestObstacle", VIPRA::Parameter{Parameter::Type::REQUIRED});
  }
};
}  // namespace VIPRA::Goals

CHECK_MODULE(GoalsModule, VIPRA::Goals::AStar)