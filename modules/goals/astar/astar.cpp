

#include "vipra/macros/errors.hpp"
#include "vipra/macros/module.hpp"

#include "astar.hpp"
#include "astar_algo.hpp"
#include "vipra/modules/pedestrians.hpp"
#include "vipra/random/random.hpp"

VIPRA_REGISTER_MODULE(VIPRA::Goals::AStar, Goals)

namespace VIPRA::Goals {

void AStar::init_step(VIPRA::Modules::Pedestrians const& pedset,
                      VIPRA::Modules::Map const& map, VIPRA::Random::Engine& engine)
{
  VIPRA::size const pedCnt = pedset.num_pedestrians();

  // Create map graph
  _graph = PathingGraph(map, _gridSize, _closestObstacle);
  set_end_goals(pedset, map, engine);

  _paths.clear();
  _paths.resize(pedCnt);

  // for each pedestrian, find their path to their end goal
  for ( VIPRA::idx pedIdx = 0; pedIdx < pedCnt; ++pedIdx ) {
    find_path(pedIdx, pedset.ped_coords(pedIdx));
    VIPRA::Log::debug("Found Path for Ped {}", pedIdx);
    set_current_goal(pedIdx, _paths[pedIdx].back());
  }

  assert(_paths.size() == pedCnt);
}

void AStar::reset_module() {}

void AStar::update_step(VIPRA::Modules::Pedestrians const& pedset,
                        VIPRA::Modules::Map const& map, VIPRA::delta_t deltaT)
{
}

// NOLINTNEXTLINE(misc-unused-parameters)
auto AStar::next_goal(VIPRA::idx pedIdx, VIPRA::Modules::Pedestrians const& /*pedset*/,
                      VIPRA::Modules::Map const& /*map*/,
                      VIPRA::delta_t /*deltaT*/) -> bool
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
// NOLINTNEXTLINE(misc-unused-parameters)
void AStar::change_end_goal_impl(VIPRA::idx pedIdx, VIPRA::f3d pos,
                                 VIPRA::f3d /*newGoal*/,
                                 VIPRA::Random::Engine& /*engine*/)
{
  // uses A* to find the path to the new end goal
  find_path(pedIdx, pos);
  set_current_goal(pedIdx, _paths[pedIdx].back());
}

auto AStar::find_random_point(VIPRA::Geometry::Polygon const& polygon,
                              VIPRA::Modules::Map const&      map,
                              VIPRA::Random::Engine&          engine) const -> f3d
{
  constexpr size_t MAX_RETRIES = 20;
  size_t           retries = 0;

  f3d point = polygon.random_point(engine);
  while ( map.collision(Geometry::Circle{point, _closestObstacle}) ) {
    point = polygon.random_point(engine);
    ++retries;
    if ( retries > MAX_RETRIES ) {
      VIPRA_MODULE_ERROR(
          "Unable to obtain a point in the objective centered at ({}, {}). "
          "Make sure all objectives are outside of obstacles.",
          polygon.center().x, polygon.center().y);
    }
  }

  return point;
}

/**
   * @brief Sets the end goals for each pedestrian
   *
   * @param pedset
   * @param map
   */
void AStar::set_end_goals(VIPRA::Modules::Pedestrians const& pedset,
                          VIPRA::Modules::Map const&         map,
                          VIPRA::Random::Engine& /*engine*/)
{
  assert(pedset.num_pedestrians() > 0);

  const VIPRA::size pedCnt = pedset.num_pedestrians();

  // find the end goals, provided as a module parameter
  auto const& objects = map.get_objectives(_endGoalType);
  if ( objects.empty() ) {
    VIPRA_MODULE_ERROR("No objects of type {} found in map", _endGoalType);
  }

  // set each pedestrians end goal as the nearest of that type
  for ( VIPRA::idx pedIdx = 0; pedIdx < pedCnt; ++pedIdx ) {
    auto const pos = pedset.ped_coords(pedIdx);

    auto const nearestGoalIter = get_nearest_goal(pos, objects);
    VIPRA::Log::debug("Ped {} Nearest End Goal: ({}, {})", pedIdx,
                      (*nearestGoalIter).center().x, (*nearestGoalIter).center().y);

    if ( nearestGoalIter != objects.end() ) {
      // f3d point = find_random_point(*nearestGoalIter, map, engine);
      f3d point = nearestGoalIter->center();

      VIPRA::Log::debug("Ped {} Final End Goal: ({}, {})", pedIdx, point.x, point.y);
      set_end_goal(pedIdx, point);
    }
    else {
      VIPRA_MODULE_ERROR(
          "No goal of type {} found for pedestrian {}. Check spelling and that "
          "the provided map contains {}",
          _endGoalType, pedIdx, _endGoalType);
    }
  }
}

/**
   * @brief Gets the nearest goal to the pedestrian
   *
   * @param pos
   * @param goals
   * @return VIPRA::f3dVec::const_iterator
   */
auto AStar::get_nearest_goal(VIPRA::f3d                                   pos,
                             std::vector<VIPRA::Geometry::Polygon> const& goals)
    -> std::vector<VIPRA::Geometry::Polygon>::const_iterator
{
  assert(goals.empty() == false);

  return std::min_element(
      goals.begin(), goals.end(), [&](auto const& left, auto const& right) {
        return pos.distance_to(left.center()) < pos.distance_to(right.center());
      });
}

/**
   * @brief Sets the pedestrians path to pos
   * 
   * @param pedIdx 
   * @param pos 
   */
void AStar::find_path(VIPRA::idx pedIdx, VIPRA::f3d startPos)
{
  // Get the pedestrian start and end grid location
  VIPRA::idx startIdx = _graph.get_closest_grid_idx(startPos);
  VIPRA::idx endIdx = _graph.get_closest_grid_idx(end_goal(pedIdx));

  if ( _graph.node_count() <= startIdx || _graph.node_count() <= endIdx ) {
    VIPRA_MODULE_ERROR("Pedestrian or goal is outside the bounds of the map provided");
  }

  auto path = VIPRA::astar(end_goal(pedIdx), startIdx, endIdx, _graph, _pathEpsilon);

  if ( ! path ) {
    VIPRA_MODULE_ERROR(
        "No path found for pedestrian {}, Start: ({}, {}), End: ({}, {})\nTry "
        "reducing gridSize in parameters",
        pedIdx, startPos.x, startPos.y, end_goal(pedIdx).x, end_goal(pedIdx).y);
  }

  _paths[pedIdx] = std::move(*path);
}
}  // namespace VIPRA::Goals