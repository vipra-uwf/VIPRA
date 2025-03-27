#pragma once

#include "pathing_graph.hpp"

#include "vipra/geometry/f3d.hpp"

#include "vipra/geometry/polygon.hpp"

#include "vipra/macros/goals.hpp"
#include "vipra/macros/module.hpp"
#include "vipra/macros/parameters.hpp"

#include "vipra/modules/goals.hpp"
#include "vipra/modules/pedestrians.hpp"
#include "vipra/random/random.hpp"

namespace VIPRA::Goals {

/**
 * @brief Goals module that uses the A* algorithm to find the path to the goal
 * 
 */
class AStar : public VIPRA::Modules::Module<AStar>, public VIPRA::Modules::Goals {
 public:
  VIPRA_MODULE_NAME("AStar")
  VIPRA_MODULE_TYPE(Goals)

  VIPRA_REGISTER_PARAMS(VIPRA_PARAM("endGoalType", _endGoalType),
                        VIPRA_PARAM("goalRange", _goalRange),
                        VIPRA_PARAM("gridSize", _gridSize),
                        VIPRA_PARAM("closestObstacle", _closestObstacle),
                        VIPRA_PARAM("pathSmoothing", _pathEpsilon))

  VIPRA_GOALS_INIT_STEP;
  VIPRA_GOALS_UPDATE_STEP;
  VIPRA_GOALS_NEXT_GOAL;
  VIPRA_GOALS_CHANGE_GOAL;
  VIPRA_GOALS_RESET;

 private:
  std::string  _endGoalType;
  VIPRA::f_pnt _goalRange;
  VIPRA::f_pnt _gridSize;
  VIPRA::f_pnt _closestObstacle;
  VIPRA::f_pnt _pathEpsilon;

  std::vector<std::vector<VIPRA::f3d>> _paths;
  PathingGraph                         _graph;

  void set_end_goals(VIPRA::Modules::Pedestrians const& pedset,
                     VIPRA::Modules::Map const& map, VIPRA::Random::Engine& engine);

  [[nodiscard]] static auto get_nearest_goal(
      VIPRA::f3d pos, std::vector<VIPRA::Geometry::Polygon> const& goals)
      -> std::vector<VIPRA::Geometry::Polygon>::const_iterator;

  void               find_path(VIPRA::idx pedIdx, VIPRA::f3d startPos);
  [[nodiscard]] auto find_random_point(VIPRA::Geometry::Polygon const&,
                                       VIPRA::Modules::Map const&,
                                       VIPRA::Random::Engine&) const -> VIPRA::f3d;
};
}  // namespace VIPRA::Goals
