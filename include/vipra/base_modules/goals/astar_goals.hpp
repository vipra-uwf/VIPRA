#pragma once

#include <cstdio>

#include "vipra/concepts/parameters.hpp"
#include "vipra/modules.hpp"
#include "vipra/types/parameter.hpp"

namespace VIPRA::Goals {
class AStar {
  VIPRA_MODULE_TYPE(GOALS)
 public:
  template <typename params_t>
  void set_params(params_t& params) {
    params.set_param(MODULE_TYPE, "endGoalType", VIPRA::Parameter{Parameter::Type::REQUIRED});
    params.set_param(MODULE_TYPE, "goalRange", VIPRA::Parameter{Parameter::Type::REQUIRED});
    params.set_param(MODULE_TYPE, "gridSize", VIPRA::Parameter{Parameter::Type::REQUIRED});
    params.set_param(MODULE_TYPE, "closestObstacle", VIPRA::Parameter{Parameter::Type::REQUIRED});
  }
};
}  // namespace VIPRA::Goals