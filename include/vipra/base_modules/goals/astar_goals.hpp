#pragma once

#include "vipra/base_modules/goals/pathing_graph.hpp"
#include "vipra/data_structures/graph.hpp"

#include "vipra/geometry/circle.hpp"
#include "vipra/geometry/f3d.hpp"

#include "vipra/macros/goals.hpp"
#include "vipra/macros/module.hpp"
#include "vipra/macros/parameters.hpp"

#include "vipra/macros/performance.hpp"
#include "vipra/modules/goals.hpp"
#include "vipra/types/size.hpp"

namespace VIPRA::Goals {
/**
 * @brief Goals module that uses the A* algorithm to find the path to the goal
 * 
 */
class AStar : public Modules::Module<AStar>, public Modules::Goals<AStar> {
 public:
  VIPRA_MODULE_NAME("astar");
  VIPRA_MODULE_TYPE(GOALS);

  VIPRA_REGISTER_PARAMS(VIPRA_PARAM("endGoalType", _endGoalType), VIPRA_PARAM("goalRange", _goalRange),
                        VIPRA_PARAM("gridSize", _gridSize), VIPRA_PARAM("closestObstacle", _closestObstacle))

  VIPRA_GOALS_INIT_STEP
  {
    VIPRA_PERF_FUNCTION("astar::init");

    assert(pedset.num_pedestrians() > 0);

    VIPRA::size const pedCnt = pedset.num_pedestrians();

    _currentGoals.resize(pedCnt);
    _endGoals.resize(pedCnt);
    _timeSinceLastGoal = std::vector<VIPRA::f_pnt>(pedCnt, 0);

    // Create map graph
    _graph = PathingGraph(map, _gridSize, _closestObstacle);
    set_end_goals(pedset, map);
    _paths.clear();
    _paths.resize(pedCnt);

    // for each pedestrian, find their path to their end goal
    for ( VIPRA::idx pedIdx = 0; pedIdx < pedCnt; ++pedIdx ) {
      find_path(pedIdx, pedset.ped_coords(pedIdx));
    }

    assert(_timeSinceLastGoal.size() == pedCnt);
    assert(_currentGoals.size() == pedCnt);
    assert(_endGoals.size() == pedCnt);
    assert(_paths.size() == pedCnt);
  }

  VIPRA_GOALS_UPDATE_STEP
  {
    VIPRA_PERF_FUNCTION("astar::update");

    assert(pedset.num_pedestrians() > 0);

    for ( VIPRA::idx pedIdx = 0; pedIdx < pedset.num_pedestrians(); ++pedIdx ) {
      auto const pos = pedset.ped_coords(pedIdx);
      _timeSinceLastGoal[pedIdx] += deltaT;

      if ( pos.distance_to(_currentGoals[pedIdx]) < _goalRange ) {
        if ( ! _paths[pedIdx].empty() ) {
          _currentGoals[pedIdx] = _paths[pedIdx].back();
          _paths[pedIdx].pop_back();
          _timeSinceLastGoal[pedIdx] = 0.0F;
        }
      }
    }
  }

  /**
     * @brief Changes a pedestrians end goal location
     *
     * @param pedIdx
     * @param newGoal
     */
  void change_end_goal(VIPRA::idx pedIdx, VIPRA::f3d pos, VIPRA::f3d newGoal)
  {
    assert(pedIdx < _endGoals.size());

    // uses A* to find the path to the new end goal
    _endGoals[pedIdx] = newGoal;
    find_path(pedIdx, pos);
  }

  [[nodiscard]] auto end_goals() const -> VIPRA::f3dVec const& { return _endGoals; }
  [[nodiscard]] auto current_goals() const -> VIPRA::f3dVec const& { return _currentGoals; }
  [[nodiscard]] auto current_goal(VIPRA::idx pedIdx) const -> VIPRA::f3d const&
  {
    assert(pedIdx < _currentGoals.size());
    return _currentGoals[pedIdx];
  }
  [[nodiscard]] auto end_goal(VIPRA::idx pedIdx) const -> VIPRA::f3d const&
  {
    assert(pedIdx < _endGoals.size());
    return _endGoals[pedIdx];
  }
  [[nodiscard]] auto is_goal_met(VIPRA::idx pedIdx) const -> bool
  {
    assert(pedIdx < _paths.size());
    return _paths[pedIdx].empty();
  }
  [[nodiscard]] auto is_sim_goal_met() const -> bool
  {
    return std::all_of(_paths.begin(), _paths.end(), [](auto const& path) { return path.empty(); });
  }

  [[nodiscard]] auto time_since_last_goal(VIPRA::idx pedIdx) const -> VIPRA::f_pnt
  {
    assert(_timeSinceLastGoal.size() > pedIdx);
    return _timeSinceLastGoal[pedIdx];
  }

 private:
  std::string  _endGoalType;
  VIPRA::f_pnt _goalRange;
  VIPRA::f_pnt _gridSize;
  VIPRA::f_pnt _closestObstacle;

  VIPRA::f3dVec             _currentGoals;
  VIPRA::f3dVec             _endGoals;
  std::vector<VIPRA::f_pnt> _timeSinceLastGoal;

  std::vector<std::vector<VIPRA::f3d>> _paths;
  PathingGraph                         _graph;

  /**
   * @brief Sets the end goals for each pedestrian
   *
   * @param pedset
   * @param map
   */
  void set_end_goals(auto const& pedset, auto const& map)
  {
    assert(pedset.num_pedestrians() > 0);

    const VIPRA::size pedCnt = pedset.num_pedestrians();

    // find the end goals, provided as a module parameter
    auto const& objects = map.get_objects(_endGoalType);
    if ( objects.empty() ) {
      throw std::runtime_error("No objects of type " + _endGoalType + " found in map");
    }

    // set each pedestrians end goal as the nearest of that type
    for ( VIPRA::idx pedIdx = 0; pedIdx < pedCnt; ++pedIdx ) {
      auto const pos = pedset.ped_coords(pedIdx);

      auto const nearestGoal = get_nearest_goal(pos, objects);

      if ( nearestGoal != objects.end() ) {
        _endGoals[pedIdx] = *nearestGoal;
      }
      else {
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
  [[nodiscard]] static auto squash_path(std::vector<VIPRA::f3d> const& path) -> std::vector<VIPRA::f3d>
  {
    assert(path.empty() == false);

    std::vector<VIPRA::f3d> squashedPath;

    VIPRA::f3d dif;
    for ( VIPRA::idx i = 1; i < path.size(); ++i ) {
      auto currDif = path[i] - path[i - 1];
      if ( currDif != dif ) {
        squashedPath.push_back(path[i - 1]);
        dif = currDif;
      }
    }

    return squashedPath;
  }

  /**
   * @brief Gets the nearest goal to the pedestrian
   *
   * @param pos
   * @param goals
   * @return VIPRA::f3dVec::const_iterator
   */
  [[nodiscard]] static auto get_nearest_goal(VIPRA::f3d pos, std::vector<VIPRA::f3d> const& goals)
      -> VIPRA::f3dVec::const_iterator
  {
    assert(goals.empty() == false);

    return std::min_element(goals.begin(), goals.end(), [&](auto const& left, auto const& right) {
      return pos.distance_to(left) < pos.distance_to(right);
    });
  }

  /**
   * @brief Sets the pedestrians path to pos
   * 
   * @param pedIdx 
   * @param pos 
   */
  void find_path(VIPRA::idx pedIdx, VIPRA::f3d startPos)
  {
    // Get the pedestrian start and end grid location
    VIPRA::idx startIdx = _graph.get_closest_grid_idx(startPos);
    VIPRA::idx endIdx = _graph.get_closest_grid_idx(_endGoals[pedIdx]);
    if ( _graph.nodes().size() <= startIdx || _graph.nodes().size() <= endIdx ) {
      throw std::runtime_error("Start or end index is out of bounds");
    }

    // use A* to find the path
    auto const path = VIPRA::Algo::astar(
        startIdx, endIdx, _graph,
        [&](VIPRA::idx left, VIPRA::idx right) -> VIPRA::f_pnt {
          return _graph.data(left).pos.distance_to(_graph.data(right).pos);
        },
        [&](VIPRA::idx nodeIdx) -> VIPRA::f3d { return _graph.data(nodeIdx).pos; });

    if ( ! path ) {
      // No path found
      throw std::runtime_error("No path found for pedestrian");
    }

    // set their path, squash nodes that all go in the same direction into one node
    _paths[pedIdx] = squash_path(*path);
    _currentGoals[pedIdx] = _paths[pedIdx].back();
  }
};
}  // namespace VIPRA::Goals
