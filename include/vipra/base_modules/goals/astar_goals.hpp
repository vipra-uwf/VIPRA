#pragma once

#include <algorithm>
#include <cstdio>
#include <queue>

#include "vipra/concepts/goals.hpp"
#include "vipra/concepts/obstacle_set.hpp"
#include "vipra/concepts/parameters.hpp"

#include "vipra/data_structures/graph.hpp"
#include "vipra/modules.hpp"
#include "vipra/types/float.hpp"
#include "vipra/types/parameter.hpp"

namespace VIPRA::Goals {
class AStar {
  VIPRA_MODULE_TYPE(GOALS)
 public:
  template <Concepts::PedsetModule pedset_t, Concepts::MapModule map_t>
  void initialize(const pedset_t& pedset, const map_t& map) {
    // TODO(rolland): implement
    _currentGoals.resize(pedset.size());
    _endGoals.resize(pedset.size());
    _paths.resize(pedset.size());

    construct_graph(map);
  }

  template <Concepts::ParamModule params_t>
  static void register_params() {
    params_t::register_param(MODULE_TYPE, "endGoalType", ParameterType::REQUIRED);
    params_t::register_param(MODULE_TYPE, "goalRange", ParameterType::REQUIRED);
    params_t::register_param(MODULE_TYPE, "gridSize", ParameterType::REQUIRED);
    params_t::register_param(MODULE_TYPE, "closestObstacle", ParameterType::REQUIRED);
  }

  template <Concepts::ParamModule params_t>
  void setup(const params_t& params) {
    _end_goal_type = params.template get_param<std::string>(MODULE_TYPE, "endGoalType");
    _goal_range = params.template get_param<VIPRA::f_pnt>(MODULE_TYPE, "goalRange");
    _grid_size = params.template get_param<VIPRA::f_pnt>(MODULE_TYPE, "gridSize");
    _closest_obstacle = params.template get_param<VIPRA::f_pnt>(MODULE_TYPE, "closestObstacle");
  }

  [[nodiscard]] auto end_goals() -> const VIPRA::f3dVec& { return _endGoals; }
  [[nodiscard]] auto current_goals() -> const VIPRA::f3dVec& { return _currentGoals; }
  [[nodiscard]] auto current_goal(VIPRA::idx pedIdx) -> VIPRA::f3d { return _currentGoals[pedIdx]; }
  [[nodiscard]] auto end_goal(VIPRA::idx pedIdx) -> VIPRA::f3d { return _endGoals[pedIdx]; }
  [[nodiscard]] auto is_goal_met(VIPRA::idx pedIdx) -> bool { return _paths[pedIdx].empty(); }
  [[nodiscard]] auto is_sim_goal_met() -> bool {
    return std::all_of(_paths.begin(), _paths.end(), [](const auto& path) { return path.empty(); });
  }

 private:
  std::string  _end_goal_type;
  VIPRA::f_pnt _goal_range;
  VIPRA::f_pnt _grid_size;
  VIPRA::f_pnt _closest_obstacle;

  VIPRA::f3dVec _currentGoals;
  VIPRA::f3dVec _endGoals;

  std::vector<std::queue<VIPRA::f3d>>      _paths;
  VIPRA::DataStructures::Graph<VIPRA::f3d> _graph;

  void construct_graph(const Concepts::MapModule auto& map) {
    const VIPRA::f3d dimensions = map.dimensions();

    const auto xSize = static_cast<VIPRA::idx>(dimensions.x / _grid_size);
    const auto ySize = static_cast<VIPRA::idx>(dimensions.y / _grid_size);

    _graph.reserve(xSize * ySize);

    for (VIPRA::idx currX = 0; currX < xSize; ++currX) {
      for (VIPRA::idx currY = 0; currY < ySize; ++currY) {
        const auto idx = _graph.add_node(VIPRA::f3d{static_cast<VIPRA::f_pnt>(currX) * _grid_size,
                                                    static_cast<VIPRA::f_pnt>(currY) * _grid_size, 0.0F});
        if (currX > 0) {
          _graph.add_edge(idx, idx - 1);
        }
        if (currY > 0) {
          _graph.add_edge(idx, idx - ySize);
        }
      }
    }
  }
};
}  // namespace VIPRA::Goals

CHECK_MODULE(GoalsModule, VIPRA::Goals::AStar)