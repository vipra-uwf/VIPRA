#pragma once

#include <cstdio>

#include "vipra/concepts/goals.hpp"
#include "vipra/concepts/obstacle_set.hpp"
#include "vipra/concepts/parameters.hpp"

#include "vipra/modules.hpp"
#include "vipra/types/float.hpp"
#include "vipra/types/parameter.hpp"

namespace VIPRA::Goals {
class AStar {
  VIPRA_MODULE_TYPE(GOALS)
 public:
  template <Concepts::PedsetModule pedset_t, Concepts::MapModule map_t>
  void initialize(const pedset_t& pedset, const map_t& obstacles) {
    // TODO(rolland): implement
  }

  template <Concepts::ParamModule params_t>
  static void register_params() {
    params_t::register_param(MODULE_TYPE, "endGoalType", VIPRA::Parameter{Parameter::Type::REQUIRED});
    params_t::register_param(MODULE_TYPE, "goalRange", VIPRA::Parameter{Parameter::Type::REQUIRED});
    params_t::register_param(MODULE_TYPE, "gridSize", VIPRA::Parameter{Parameter::Type::REQUIRED});
    params_t::register_param(MODULE_TYPE, "closestObstacle", VIPRA::Parameter{Parameter::Type::REQUIRED});
  }

  template <Concepts::ParamModule params_t>
  void setup(const params_t& params) {
    _end_goal_type = params.template get_param<std::string>(MODULE_TYPE, "endGoalType");
    _goal_range = params.template get_param<VIPRA::f_pnt>(MODULE_TYPE, "goalRange");
    _grid_size = params.template get_param<VIPRA::f_pnt>(MODULE_TYPE, "gridSize");
    _closest_obstacle = params.template get_param<VIPRA::f_pnt>(MODULE_TYPE, "closestObstacle");
  }

 private:
  std::string  _end_goal_type;
  VIPRA::f_pnt _goal_range;
  VIPRA::f_pnt _grid_size;
  VIPRA::f_pnt _closest_obstacle;
};
}  // namespace VIPRA::Goals

CHECK_MODULE(GoalsModule, VIPRA::Goals::AStar)