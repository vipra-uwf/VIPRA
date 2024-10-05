#pragma once

#include "pathing_graph.hpp"

#include "vipra/geometry/f3d.hpp"

#include "vipra/geometry/polygon.hpp"

#include "vipra/logging/logging.hpp"

#include "vipra/macros/goals.hpp"
#include "vipra/macros/module.hpp"
#include "vipra/macros/parameters.hpp"

#include "vipra/modules/goals.hpp"
#include "vipra/random/random.hpp"
#include "vipra/types/size.hpp"

namespace VIPRA::Goals {

/**
 * @brief Goals module that uses the A* algorithm to find the path to the goal
 * 
 */
class AStar : public VIPRA::Modules::Module<AStar>,
              public VIPRA::Modules::Goals {
 public:
  VIPRA_MODULE_NAME("AStar")
  VIPRA_MODULE_TYPE(Goals)

  VIPRA_REGISTER_PARAMS(VIPRA_PARAM("endGoalType", _endGoalType),
                        VIPRA_PARAM("goalRange", _goalRange),
                        VIPRA_PARAM("gridSize", _gridSize),
                        VIPRA_PARAM("closestObstacle", _closestObstacle))

  VIPRA_GOALS_INIT_STEP
  {
    VIPRA::size const pedCnt = pedset.num_pedestrians();

    // Create map graph
    _graph = PathingGraph(map, _gridSize, _closestObstacle);
    set_end_goals(pedset, map, engine);

    _paths.clear();
    _paths.resize(pedCnt);

    // for each pedestrian, find their path to their end goal
    for ( VIPRA::idx pedIdx = 0; pedIdx < pedCnt; ++pedIdx ) {
      find_path(pedIdx, pedset.ped_coords(pedIdx), engine);
      VIPRA::Log::debug("Found Path for Ped {}", pedIdx);
      set_current_goal(pedIdx, _paths[pedIdx].back());
    }

    assert(_paths.size() == pedCnt);
  }

  VIPRA_GOALS_UPDATE_STEP {}

  // NOLINTNEXTLINE(misc-unused-parameters)
  VIPRA_GOALS_NEXT_GOAL
  {
    if ( _paths[pedIdx].empty() ) return true;

    set_current_goal(pedIdx, _paths[pedIdx].back());
    _paths[pedIdx].pop_back();

    return false;
  }

  /**
     * @brief Changes a pedestrians end goal location
     *
     * @param pedIdx
     * @param newGoal
     */
  VIPRA_GOALS_CHANGE_GOAL
  {
    // uses A* to find the path to the new end goal
    set_end_goal(pedIdx, newGoal);
    find_path(pedIdx, pos, engine);
  }

 private:
  std::string  _endGoalType;
  VIPRA::f_pnt _goalRange;
  VIPRA::f_pnt _gridSize;
  VIPRA::f_pnt _closestObstacle;

  std::vector<std::vector<VIPRA::f3d>> _paths;
  PathingGraph                         _graph;

  /**
   * @brief Sets the end goals for each pedestrian
   *
   * @param pedset
   * @param map
   */
  void set_end_goals(auto const& pedset, auto const& map,
                     VIPRA::Random::Engine& engine)
  {
    assert(pedset.num_pedestrians() > 0);

    const VIPRA::size pedCnt = pedset.num_pedestrians();

    // find the end goals, provided as a module parameter
    auto const& objects = map.get_objectives(_endGoalType);
    if ( objects.empty() ) {
      throw std::runtime_error("No objects of type " + _endGoalType +
                               " found in map");
    }

    // set each pedestrians end goal as the nearest of that type
    for ( VIPRA::idx pedIdx = 0; pedIdx < pedCnt; ++pedIdx ) {
      auto const pos = pedset.ped_coords(pedIdx);

      auto const nearestGoalIter = get_nearest_goal(pos, objects);
      VIPRA::Log::debug("Ped {} Nearest End Goal: ({}, {})", pedIdx,
                        (*nearestGoalIter).center().x,
                        (*nearestGoalIter).center().y);

      if ( nearestGoalIter != objects.end() ) {
        const auto point = (*nearestGoalIter).random_point(engine);
        VIPRA::Log::debug("Ped {} Final End Goal: ({}, {})", pedIdx, point.x,
                          point.y);
        set_end_goal(pedIdx, point);
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
  [[nodiscard]] static auto squash_path(std::vector<VIPRA::f3d> const& path,
                                        VIPRA::Random::Engine& /*engine*/)
      -> std::vector<VIPRA::f3d>
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
  [[nodiscard]] static auto get_nearest_goal(
      VIPRA::f3d pos, std::vector<VIPRA::Geometry::Polygon> const& goals)
      -> std::vector<VIPRA::Geometry::Polygon>::const_iterator
  {
    assert(goals.empty() == false);

    return std::min_element(goals.begin(), goals.end(),
                            [&](auto const& left, auto const& right) {
                              return pos.distance_to(left.center()) <
                                     pos.distance_to(right.center());
                            });
  }

  /**
   * @brief Sets the pedestrians path to pos
   * 
   * @param pedIdx 
   * @param pos 
   */
  void find_path(VIPRA::idx pedIdx, VIPRA::f3d startPos,
                 VIPRA::Random::Engine& engine)
  {
    // Get the pedestrian start and end grid location
    VIPRA::idx startIdx = _graph.get_closest_grid_idx(startPos);
    VIPRA::idx endIdx = _graph.get_closest_grid_idx(end_goal(pedIdx));

    if ( _graph.nodes().size() <= startIdx ||
         _graph.nodes().size() <= endIdx ) {
      throw std::runtime_error("Start or end index is out of bounds");
    }

    // use A* to find the path
    auto const path = VIPRA::Algo::astar(
        startIdx, endIdx, _graph,
        [&](VIPRA::idx left, VIPRA::idx right) -> VIPRA::f_pnt {
          // distance comparison function
          return _graph.data(left).pos.distance_to(_graph.data(right).pos);
        },
        [&](VIPRA::idx nodeIdx) -> VIPRA::f3d {
          return _graph.data(nodeIdx).pos;
        });

    if ( ! path ) {
      // No path found
      throw std::runtime_error("No path found for pedestrian " +
                               std::to_string(pedIdx));
    }

    // set their path, squash nodes that all go in the same direction into one node
    _paths[pedIdx] = squash_path(*path, engine);
  }
};
}  // namespace VIPRA::Goals
