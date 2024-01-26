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

    for (VIPRA::idx pedIdx = 0; pedIdx < 1; ++pedIdx) {
      const auto pos = pedset.ped_coords(pedIdx);

      VIPRA::idx startIdx = get_closest_grid_idx(pos);
      VIPRA::idx endIdx = get_closest_grid_idx(_endGoals[pedIdx]);

      if (_graph.nodes().size() <= startIdx || _graph.nodes().size() <= endIdx) {
        throw std::runtime_error("Start or end index is out of bounds");
      }

      _paths.push_back(VIPRA::Algo::astar(
          startIdx, endIdx, _graph,
          [&](VIPRA::idx left, VIPRA::idx right) -> VIPRA::f_pnt {
            return _graph.data(left).pos.distance_to(_graph.data(right).pos);
          },
          [&](VIPRA::idx nodeIdx) -> VIPRA::f3d { return _graph.data(nodeIdx).pos; }));

      if (_paths.back().empty()) {
        throw std::runtime_error("No path found for pedestrian");
      }
    }

    Util::debug_do([&]() {
      std::printf("{");
      std::printf("\n\"nodes\": [\n");
      // for (VIPRA::idx nodeIdx = 0; nodeIdx < _graph.nodes().size(); ++nodeIdx) {
      //   const auto& node = _graph.data(nodeIdx);
      //   std::printf(R"(  { "pos": [%0.2f, %0.2f], "traversable": %s)", node.pos.x, node.pos.y,
      //               node.traversable ? "true" : "false");
      //   std::printf(", \"neighbors\": [");
      //   for (const auto& edge : _graph.neighbors(nodeIdx)) {
      //     std::printf(R"({"pos": [%0.2f, %0.2f]}%c)", _graph.data(edge).pos.x, _graph.data(edge).pos.y,
      //                 edge == _graph.neighbors(nodeIdx).back() ? ' ' : ',');
      //   }
      //   std::printf("]}%c", nodeIdx == _graph.nodes().size() - 1 ? ' ' : ',');
      // }
      std::printf("\n],\n");
      std::printf("\n\"paths\": [\n");
      for (VIPRA::idx pedIdx = 0; pedIdx < 1; ++pedIdx) {
        std::printf(R"(  { "pedIdx": %lu, "path": [)", pedIdx);
        while (!_paths[pedIdx].empty()) {
          const auto node = _paths[pedIdx].front();
          _paths[pedIdx].pop();
          std::printf(R"( { "pos": [%0.2f, %0.2f] }%c)", node.x, node.y,
                      node == _paths[pedIdx].back() ? ' ' : ',');
        }
        std::printf("]}%c", pedIdx == 1 ? ' ' : ',');
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
  VIPRA::size                             _xCount;
  VIPRA::size                             _yCount;

  void construct_graph(const Concepts::MapModule auto& map) {
    set_grid_counts(map);
    _graph.reserve(_xCount * _yCount);

    // TODO(rolland): This starts the grid a (0, 0) and not the bottom left corner of the map, we may want to change this
    VIPRA::f3d center;

    for (VIPRA::idx currY = 0; currY < _yCount; ++currY) {
      center.x = 0.0F;
      center.y += _grid_size;
      for (VIPRA::idx currX = 0; currX < _xCount; ++currX) {
        center.x += _grid_size;
        const VIPRA::idx currIdx =
            _graph.add_node({center, map.nearest_obstacle(center).distance_to(center) > _closest_obstacle});

        if (!_graph.data(currIdx).traversable) {
          continue;
        }

        // TODO(rolland): move adjacent edge creation to a function
        if (currX > 1) {
          if (_graph.data(currIdx - 1).traversable) _graph.add_edge(currIdx, currIdx - 1);
        }
        if (currY > 1) {
          if (_graph.data(currIdx - _xCount).traversable) _graph.add_edge(currIdx, currIdx - _xCount);
        }
        if (currX > 1 && currY > 1) {
          if (_graph.data(currIdx - _xCount - 1).traversable) _graph.add_edge(currIdx, currIdx - _xCount - 1);
        }
        if (currX < _xCount - 1 && currY > 1) {
          if (_graph.data(currIdx - _xCount + 1).traversable) _graph.add_edge(currIdx, currIdx - _xCount + 1);
        }
      }
    }
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

  [[nodiscard]] static auto get_index(VIPRA::size gridX, VIPRA::size gridY, VIPRA::size xCount)
      -> VIPRA::idx {
    return gridX + (gridY * xCount);
  }

  [[nodiscard]] static auto get_nearest_goal(VIPRA::f3d pos, const std::vector<VIPRA::f3d>& goals)
      -> VIPRA::f3dVec::const_iterator {
    return std::min_element(goals.begin(), goals.end(), [&](const auto& left, const auto& right) {
      return pos.distance_to(left) < pos.distance_to(right);
    });
  }

  [[nodiscard]] auto get_closest_grid_idx(VIPRA::f3d pos) const -> VIPRA::idx {
    auto gridX = static_cast<VIPRA::idx>(std::floor(pos.x / _grid_size));
    auto gridY = static_cast<VIPRA::idx>(std::floor(pos.y / _grid_size));

    const auto idx = get_index(gridX, gridY, _xCount);

    if (idx >= _graph.nodes().size()) {
      throw std::runtime_error("Grid index is out of bounds");
    }

    return idx;
  }

  void set_grid_counts(const Concepts::MapModule auto& map) {
    const VIPRA::f3d dimensions = map.get_dimensions();

    _xCount = static_cast<VIPRA::idx>(std::ceil(dimensions.x / _grid_size) + 1);
    _yCount = static_cast<VIPRA::idx>(std::ceil(dimensions.y / _grid_size) + 1);
  }
};
}  // namespace VIPRA::Goals

CHECK_MODULE(GoalsModule, VIPRA::Goals::AStar)