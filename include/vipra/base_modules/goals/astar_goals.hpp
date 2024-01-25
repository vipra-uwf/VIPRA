#pragma once

#include <algorithm>
#include <cstdio>
#include <queue>

#include "vipra/algorithms/astar.hpp"

#include "vipra/concepts/goals.hpp"
#include "vipra/concepts/obstacle_set.hpp"
#include "vipra/concepts/parameters.hpp"

#include "vipra/data_structures/graph.hpp"

#include "vipra/modules.hpp"

#include "vipra/types/float.hpp"
#include "vipra/types/idx.hpp"
#include "vipra/types/parameter.hpp"
#include "vipra/types/size.hpp"
#include "vipra/util/debug_do.hpp"

namespace VIPRA::Goals {
class AStar {
  VIPRA_MODULE_TYPE(GOALS)
 public:
  template <Concepts::PedsetModule pedset_t, Concepts::MapModule map_t>
  void initialize(const pedset_t& pedset, const map_t& map) {
    _currentGoals.resize(pedset.num_pedestrians());
    _endGoals.resize(pedset.num_pedestrians());

    construct_graph(map);
    set_end_goals(pedset, map);

    // Util::debug_do([&]() {
    //   std::printf("{\n\"nodes\":[\n");
    //   for (auto& node : _graph.nodes()) {
    //     std::printf(R"(  { "pos": [%0.2f, %0.2f], "traversable": %s, "neighbors": [)", node.data.pos.x,
    //                 node.data.pos.y, node.data.traversable ? "true" : "false");
    //     for (auto& neighbor : node.neighbors) {
    //       std::printf("{ \"pos\": [%0.2f, %0.2f] }%c", _graph.data(neighbor).pos.x,
    //                   _graph.data(neighbor).pos.y, neighbor == node.neighbors.back() ? ' ' : ',');
    //     }
    //     std::printf("] }%c\n", node.data.pos == _graph.nodes().back().data.pos ? ' ' : ',');
    //   }
    //   std::printf("]}");
    //   std::fflush(stdout);
    // });

    for (VIPRA::idx pedIdx = 0; pedIdx < pedset.num_pedestrians(); ++pedIdx) {
      const auto pos = pedset.ped_coords(pedIdx);

      VIPRA::idx startIdx = get_closest_grid_idx(pos, map.get_dimensions());
      VIPRA::idx endIdx = get_closest_grid_idx(_endGoals[pedIdx], map.get_dimensions());

      const auto path = VIPRA::Algo::astar(
          startIdx, endIdx, _graph,
          [&](VIPRA::idx left, VIPRA::idx right) -> VIPRA::f_pnt {
            return _graph.data(left).pos.distance_to(_graph.data(right).pos);
          },
          [&](VIPRA::idx nodeIdx) -> VIPRA::f3d { return _graph.data(nodeIdx).pos; });

      if (path.empty()) {
        throw std::runtime_error("No path found for pedestrian");
      }

      _paths.push_back(std::move(path));
    }

    Util::debug_do([&]() {
      std::printf("{\n\"paths\": [\n");
      for (VIPRA::idx pedIdx = 0; pedIdx < 1; ++pedIdx) {
        std::printf(R"(  { "pedIdx": %lu, "path": [)", pedIdx);
        while (!_paths[pedIdx].empty()) {
          const auto node = _paths[pedIdx].front();
          _paths[pedIdx].pop();
          std::printf(R"( { "pos": [%0.2f, %0.2f] }%c)", node.x, node.y,
                      node == _paths[pedIdx].back() ? ' ' : ',');
        }
        std::printf("]}");
      }
      std::printf("]}");
      std::fflush(stdout);
    });

    // for (VIPRA::idx pedIdx = 0; pedIdx < pedset.num_pedestrians(); ++pedIdx) {
    //   _currentGoals[pedIdx] = _paths[pedIdx].front();
    //   _paths[pedIdx].pop();
    // }
  }

  template <Concepts::ParamModule params_t>
  void setup(const params_t& params) {
    _end_goal_type = params.template get_param<std::string>(MODULE_TYPE, "endGoalType");
    _goal_range = params.template get_param<VIPRA::f_pnt>(MODULE_TYPE, "goalRange");
    _grid_size = params.template get_param<VIPRA::f_pnt>(MODULE_TYPE, "gridSize");
    _closest_obstacle = params.template get_param<VIPRA::f_pnt>(MODULE_TYPE, "closestObstacle");
  }

  template <Concepts::ParamModule params_t>
  static void register_params() {
    params_t::register_param(MODULE_TYPE, "endGoalType", ParameterType::REQUIRED);
    params_t::register_param(MODULE_TYPE, "goalRange", ParameterType::REQUIRED);
    params_t::register_param(MODULE_TYPE, "gridSize", ParameterType::REQUIRED);
    params_t::register_param(MODULE_TYPE, "closestObstacle", ParameterType::REQUIRED);
  }

  template <Concepts::PedsetModule pedset_t, Concepts::MapModule map_t>
  void update(const pedset_t& pedset, const map_t& /*unused*/) {
    for (VIPRA::idx pedIdx = 0; pedIdx < pedset.num_pedestrians(); ++pedIdx) {
      const auto pos = pedset.ped_coords(pedIdx);

      if (pos.distance_to(_currentGoals[pedIdx]) < _goal_range) {
        if (!_paths[pedIdx].empty()) {
          _currentGoals[pedIdx] = _paths[pedIdx].front();
          _paths[pedIdx].pop();
        }
      }
    }
  }

  [[nodiscard]] auto end_goals() const -> const VIPRA::f3dVec& { return _endGoals; }
  [[nodiscard]] auto current_goals() const -> const VIPRA::f3dVec& { return _currentGoals; }
  [[nodiscard]] auto current_goal(VIPRA::idx pedIdx) const -> VIPRA::f3d { return _currentGoals[pedIdx]; }
  [[nodiscard]] auto end_goal(VIPRA::idx pedIdx) const -> VIPRA::f3d { return _endGoals[pedIdx]; }
  [[nodiscard]] auto is_goal_met(VIPRA::idx pedIdx) const -> bool { return _paths[pedIdx].empty(); }
  [[nodiscard]] auto is_sim_goal_met() const -> bool {
    return std::all_of(_paths.begin(), _paths.end(), [](const auto& path) { return path.empty(); });
  }

 private:
  struct GridPoint {
    VIPRA::f3d pos;
    bool       traversable;
  };

  std::string  _end_goal_type;
  VIPRA::f_pnt _goal_range;
  VIPRA::f_pnt _grid_size;
  VIPRA::f_pnt _closest_obstacle;

  VIPRA::f3dVec _currentGoals;
  VIPRA::f3dVec _endGoals;

  std::vector<std::queue<VIPRA::f3d>>     _paths;
  VIPRA::DataStructures::Graph<GridPoint> _graph;

  void construct_graph(const Concepts::MapModule auto& map) {
    const VIPRA::f3d dimensions = map.get_dimensions();

    const auto xSize = static_cast<VIPRA::idx>(dimensions.x / _grid_size);
    const auto ySize = static_cast<VIPRA::idx>(dimensions.y / _grid_size);

    _graph.reserve(xSize * ySize);

    for (VIPRA::idx currY = 0; currY < ySize; ++currY) {
      for (VIPRA::idx currX = 0; currX < xSize; ++currX) {
        const VIPRA::f3d currPos = VIPRA::f3d{static_cast<VIPRA::f_pnt>(currX) * _grid_size,
                                              static_cast<VIPRA::f_pnt>(currY) * _grid_size, 0.0F};
        const VIPRA::idx currIdx = _graph.add_node(
            {currPos, map.nearest_obstacle(currPos).distance_to(currPos) > _closest_obstacle});

        if (!_graph.data(currIdx).traversable) {
          continue;
        }

        // TODO(rolland): check if grid is traversable

        if (currX > 1) {
          if (_graph.data(currIdx - 1).traversable) _graph.add_edge(currIdx, currIdx - 1);
        }
        if (currY > 1) {
          if (_graph.data(currIdx - xSize).traversable) _graph.add_edge(currIdx, currIdx - xSize);
        }
        if (currX > 1 && currY > 1) {
          if (_graph.data(currIdx - xSize - 1).traversable) _graph.add_edge(currIdx, currIdx - xSize - 1);
        }
        if (currX > 1 && currY > ySize && currX < xSize - 1) {
          if (_graph.data(currIdx - xSize + 1).traversable) _graph.add_edge(currIdx, currIdx - xSize + 1);
        }
      }
    }
  }

  [[nodiscard]] constexpr auto get_closest_grid_idx(VIPRA::f3d pos, VIPRA::f3d dims) const -> VIPRA::idx {
    if (pos.x < 0.0F || pos.x > dims.x || pos.y < 0.0F || pos.y > dims.y) {
      throw std::runtime_error("Position is outside of map at: " + pos.to_string());
    }

    const auto xSize = static_cast<VIPRA::idx>(pos.x / _grid_size);
    const auto ySize = static_cast<VIPRA::idx>(pos.y / _grid_size);
    return xSize + ySize * static_cast<VIPRA::idx>(dims.x / _grid_size);
  }

  [[nodiscard]] static auto get_nearest_goal(VIPRA::f3d pos, const std::vector<VIPRA::f3d>& goals) {
    return std::min_element(goals.begin(), goals.end(), [&](const auto& left, const auto& right) {
      return pos.distance_to(left) < pos.distance_to(right);
    });
  }

  void set_end_goals(const Concepts::PedsetModule auto& pedset, const Concepts::MapModule auto& map) {
    const VIPRA::size pedCnt = pedset.num_pedestrians();

    const auto& objects = map.get_objects(_end_goal_type);
    if (objects.empty()) {
      throw std::runtime_error("No objects of type " + _end_goal_type + " found in map");
    }

    for (VIPRA::idx pedIdx = 0; pedIdx < pedCnt; ++pedIdx) {
      const auto pos = pedset.ped_coords(pedIdx);

      const auto nearestGoal = get_nearest_goal(pos, objects);

      if (nearestGoal != objects.end()) {
        _endGoals[pedIdx] = *nearestGoal;
      } else {
        throw std::runtime_error("No goal found for pedestrian");
      }
    }
  }
};
}  // namespace VIPRA::Goals

CHECK_MODULE(GoalsModule, VIPRA::Goals::AStar)