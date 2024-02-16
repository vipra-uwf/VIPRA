#pragma once

#include <algorithm>
#include <cstdio>
#include <queue>

#include "vipra/algorithms/astar.hpp"

#include "vipra/concepts/goals.hpp"
#include "vipra/concepts/obstacle_set.hpp"

#include "vipra/data_structures/graph.hpp"

#include "vipra/macros/module.hpp"
#include "vipra/modules.hpp"

#include "vipra/types/float.hpp"
#include "vipra/types/idx.hpp"
#include "vipra/types/parameter.hpp"
#include "vipra/types/size.hpp"

#include "vipra/types/time.hpp"
#include "vipra/util/debug_do.hpp"

namespace VIPRA::Goals {
class AStar {
 public:
  VIPRA_MODULE_TYPE(GOALS);

  template <Concepts::PedsetModule pedset_t, Concepts::MapModule map_t>
  void initialize(pedset_t const& pedset, map_t const& map) {
    assert(pedset.num_pedestrians() > 0);

    auto const pedCnt = pedset.num_pedestrians();

    _currentGoals.resize(pedCnt);
    _endGoals.resize(pedCnt);
    _timeSinceLastGoal = std::vector<VIPRA::f_pnt>(pedCnt, 0);

    construct_graph(map);
    set_end_goals(pedset, map);

    for (VIPRA::idx pedIdx = 0; pedIdx < pedCnt; ++pedIdx) {
      auto const pos = pedset.ped_coords(pedIdx);

      VIPRA::idx startIdx = get_closest_grid_idx(pos);
      VIPRA::idx endIdx = get_closest_grid_idx(_endGoals[pedIdx]);

      if (_graph.nodes().size() <= startIdx || _graph.nodes().size() <= endIdx) {
        throw std::runtime_error("Start or end index is out of bounds");
      }

      auto const path = VIPRA::Algo::astar(
          startIdx, endIdx, _graph,
          [&](VIPRA::idx left, VIPRA::idx right) -> VIPRA::f_pnt {
            return _graph.data(left).pos.distance_to(_graph.data(right).pos);
          },
          [&](VIPRA::idx nodeIdx) -> VIPRA::f3d { return _graph.data(nodeIdx).pos; });

      if (!path) {
        throw std::runtime_error("No path found for pedestrian");
      }

      _paths.push_back(squash_path(path.value()));
    }

    for (VIPRA::idx pedIdx = 0; pedIdx < pedCnt; ++pedIdx) {
      _currentGoals[pedIdx] = _paths[pedIdx].back();
    }

    assert(_timeSinceLastGoal.size() == pedCnt);
    assert(_currentGoals.size() == pedCnt);
    assert(_endGoals.size() == pedCnt);
    assert(_paths.size() == pedCnt);
  }

  template <typename params_t>
  void config(params_t const& params, VIPRA::Random::Engine& engine) {
    _end_goal_type =
        params.template get_param<std::string>(_VIPRA_MODULE_TYPE_, "astar", "endGoalType", engine);
    _goal_range = params.template get_param<VIPRA::f_pnt>(_VIPRA_MODULE_TYPE_, "astar", "goalRange", engine);
    _grid_size = params.template get_param<VIPRA::f_pnt>(_VIPRA_MODULE_TYPE_, "astar", "gridSize", engine);
    _closest_obstacle =
        params.template get_param<VIPRA::f_pnt>(_VIPRA_MODULE_TYPE_, "astar", "closestObstacle", engine);
  }

  template <typename params_t>
  void register_params(params_t& params) {
    params.register_param(_VIPRA_MODULE_TYPE_, "astar", "endGoalType");
    params.register_param(_VIPRA_MODULE_TYPE_, "astar", "goalRange");
    params.register_param(_VIPRA_MODULE_TYPE_, "astar", "gridSize");
    params.register_param(_VIPRA_MODULE_TYPE_, "astar", "closestObstacle");
  }

  template <Concepts::PedsetModule pedset_t, Concepts::MapModule map_t>
  void update(pedset_t const& pedset, map_t const& /*unused*/, VIPRA::delta_t deltaT) {
    assert(pedset.num_pedestrians() > 0);

    for (VIPRA::idx pedIdx = 0; pedIdx < pedset.num_pedestrians(); ++pedIdx) {
      auto const pos = pedset.ped_coords(pedIdx);
      _timeSinceLastGoal[pedIdx] += deltaT;
      if (pos.distance_to(_currentGoals[pedIdx]) < _goal_range) {
        if (!_paths[pedIdx].empty()) {
          _currentGoals[pedIdx] = _paths[pedIdx].back();
          _paths[pedIdx].pop_back();
          _timeSinceLastGoal[pedIdx] = 0.0F;
        }
      }
    }
  }

  void change_end_goal(VIPRA::idx pedIdx, VIPRA::f3d newGoal) {
    assert(pedIdx < _endGoals.size());
    _endGoals[pedIdx] = newGoal;
    auto const path = VIPRA::Algo::astar(
        get_closest_grid_idx(_currentGoals[pedIdx]), get_closest_grid_idx(_endGoals[pedIdx]), _graph,
        [&](VIPRA::idx left, VIPRA::idx right) -> VIPRA::f_pnt {
          return _graph.data(left).pos.distance_to(_graph.data(right).pos);
        },
        [&](VIPRA::idx nodeIdx) -> VIPRA::f3d { return _graph.data(nodeIdx).pos; });

    if (!path) {
      throw std::runtime_error("No path found for pedestrian");
    }

    _paths[pedIdx] = squash_path(*path);
    _currentGoals[pedIdx] = _paths[pedIdx].back();
  }

  [[nodiscard]] auto end_goals() const -> VIPRA::f3dVec const& { return _endGoals; }
  [[nodiscard]] auto current_goals() const -> VIPRA::f3dVec const& { return _currentGoals; }
  [[nodiscard]] auto current_goal(VIPRA::idx pedIdx) const -> VIPRA::f3d const& {
    assert(pedIdx < _currentGoals.size());
    return _currentGoals[pedIdx];
  }
  [[nodiscard]] auto end_goal(VIPRA::idx pedIdx) const -> VIPRA::f3d const& {
    assert(pedIdx < _endGoals.size());
    return _endGoals[pedIdx];
  }
  [[nodiscard]] auto is_goal_met(VIPRA::idx pedIdx) const -> bool {
    assert(pedIdx < _paths.size());
    return _paths[pedIdx].empty();
  }
  [[nodiscard]] auto is_sim_goal_met() const -> bool {
    return std::all_of(_paths.begin(), _paths.end(), [](auto const& path) { return path.empty(); });
  }

  [[nodiscard]] auto time_since_last_goal(VIPRA::idx pedIdx) const -> VIPRA::f_pnt {
    assert(_timeSinceLastGoal.size() > pedIdx);
    return _timeSinceLastGoal[pedIdx];
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

  VIPRA::f3dVec             _currentGoals;
  VIPRA::f3dVec             _endGoals;
  std::vector<VIPRA::f_pnt> _timeSinceLastGoal;

  std::vector<std::vector<VIPRA::f3d>>    _paths;
  VIPRA::DataStructures::Graph<GridPoint> _graph;
  VIPRA::size                             _xCount;
  VIPRA::size                             _yCount;

  /**
   * @brief Constructs the graph of grid points
   * 
   * @param map 
   */
  void construct_graph(Concepts::MapModule auto const& map) {
    set_grid_counts(map);

    assert(_xCount > 0 && _yCount > 0);

    _graph.reserve(_xCount * _yCount);

    // TODO(rolland): This starts the grid a (0, 0) and not the bottom left corner of the map, we may want to change this
    VIPRA::f3d center;

    for (VIPRA::idx currY = 0; currY < _yCount; ++currY) {
      center.x = 0.0F;
      center.y += _grid_size;

      for (VIPRA::idx currX = 0; currX < _xCount; ++currX) {
        center.x += _grid_size;
        const VIPRA::idx currIdx = _graph.add_node(
            GridPoint{center, !map.collision(VIPRA::Geometry::Circle{center, _closest_obstacle})});

        if (!_graph.data(currIdx).traversable) {
          continue;
        }

        set_adjacents(currIdx, currX, currY);
      }
    }
  }

  /**
   * @brief Set the adjacent nodes of the current node
   * 
   * @param currIdx 
   * @param currX 
   * @param currY 
   */
  // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
  void set_adjacents(VIPRA::idx currIdx, VIPRA::idx currX, VIPRA::idx currY) {
    assert(currIdx < _graph.nodes().size());
    assert(currX < _xCount && currY < _yCount);
    assert(_graph.data(currIdx).traversable == true);

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

  /**
   * @brief Sets the end goals for each pedestrian
   * 
   * @param pedset 
   * @param map 
   */
  void set_end_goals(Concepts::PedsetModule auto const& pedset, Concepts::MapModule auto const& map) {
    assert(pedset.num_pedestrians() > 0);

    const VIPRA::size pedCnt = pedset.num_pedestrians();

    auto const& objects = map.get_objects(_end_goal_type);
    if (objects.empty()) {
      throw std::runtime_error("No objects of type " + _end_goal_type + " found in map");
    }

    for (VIPRA::idx pedIdx = 0; pedIdx < pedCnt; ++pedIdx) {
      auto const pos = pedset.ped_coords(pedIdx);

      auto const nearestGoal = get_nearest_goal(pos, objects);

      if (nearestGoal != objects.end()) {
        _endGoals[pedIdx] = *nearestGoal;
      } else {
        throw std::runtime_error("No goal found for pedestrian");
      }
    }
  }

  /**
   * @brief Squashes the path to remove unnecessary points
   * 
   * @param path 
   * @return std::vector<VIPRA::f3d> 
   */
  [[nodiscard]] static auto squash_path(std::vector<VIPRA::f3d> const& path) -> std::vector<VIPRA::f3d> {
    assert(path.empty() == false);

    std::vector<VIPRA::f3d> squashedPath;
    squashedPath.reserve(path.size() / 2);

    VIPRA::f3d dif;
    for (VIPRA::idx i = 1; i < path.size(); ++i) {
      auto currDif = path[i] - path[i - 1];
      if (currDif != dif) {
        squashedPath.push_back(path[i - 1]);
        dif = currDif;
      }
    }

    return squashedPath;
  }

  /**
   * @brief Gets the index of the grid point
   * 
   * @param gridX 
   * @param gridY 
   * @param xCount 
   * @return VIPRA::idx 
   */
  [[nodiscard]] static auto get_index(VIPRA::size gridX, VIPRA::size gridY, VIPRA::size xCount) noexcept
      -> VIPRA::idx {
    return gridX + (gridY * xCount);
  }

  /**
   * @brief Gets the nearest goal to the pedestrian
   * 
   * @param pos 
   * @param goals 
   * @return VIPRA::f3dVec::const_iterator 
   */
  [[nodiscard]] static auto get_nearest_goal(VIPRA::f3d pos, std::vector<VIPRA::f3d> const& goals)
      -> VIPRA::f3dVec::const_iterator {
    assert(goals.empty() == false);

    return std::min_element(goals.begin(), goals.end(), [&](auto const& left, auto const& right) {
      return pos.distance_to(left) < pos.distance_to(right);
    });
  }

  /**
   * @brief Gets the closest grid index to the coordinate
   * 
   * @param pos 
   * @return VIPRA::idx 
   */
  [[nodiscard]] auto get_closest_grid_idx(VIPRA::f3d pos) const -> VIPRA::idx {
    auto gridX = static_cast<VIPRA::idx>(std::floor(pos.x / _grid_size));
    auto gridY = static_cast<VIPRA::idx>(std::floor(pos.y / _grid_size));

    auto const idx = get_index(gridX, gridY, _xCount);

    if (idx >= _graph.nodes().size()) {
      throw std::runtime_error("Grid index is out of bounds");
    }

    return idx;
  }

  /**
   * @brief Sets the number of grid points in the x and y directions
   * 
   * @param map 
   */
  void set_grid_counts(const Concepts::MapModule auto& map) {
    const VIPRA::f3d dimensions = map.get_dimensions();

    assert(dimensions.x > 0 && dimensions.y > 0);
    assert(_grid_size > 0 && _closest_obstacle > 0);

    _xCount = static_cast<VIPRA::idx>(std::ceil(dimensions.x / _grid_size) + 1);
    _yCount = static_cast<VIPRA::idx>(std::ceil(dimensions.y / _grid_size) + 1);
  }

  // void debug_output_paths() {
  //   std::printf("{\"paths\": [\n");
  //   for (auto const& path : _paths) {
  //     std::printf("{ \"points\": [\n");
  //     for (auto const& point : path) {
  //       std::printf("[%f, %f, %f]%c\n", point.x, point.y, point.z, point == path.back() ? ' ' : ',');
  //     }
  //     std::printf("]}%c\n", &path == &_paths.back() ? ' ' : ',');
  //   }
  //   std::printf("]}\n");
  //   std::fflush(stdout);
  // }
};
}  // namespace VIPRA::Goals

CHECK_MODULE(GoalsModule, VIPRA::Goals::AStar)