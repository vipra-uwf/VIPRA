
#include "calm_collision.hpp"

namespace CALM {

void CollisionDetection::initialize(VIPRA::f_pnt                       range,
                                    VIPRA::Modules::Pedestrians const& pedset)
{
  _range = range;
  _collisionRects.resize(pedset.num_pedestrians());
  _statuses.resize(pedset.num_pedestrians());
}

void CollisionDetection::race_detection(VIPRA::Modules::Pedestrians const& pedset,
                                        VIPRA::Modules::Goals const&       goals,
                                        CALM::ModelData const&             modelData)
{
  calc_collision_shapes(pedset, modelData);
  calc_races(pedset, goals);
}

void CollisionDetection::calc_races(VIPRA::Modules::Pedestrians const& pedset,
                                    VIPRA::Modules::Goals const&       goals)
{
  VIPRA::size const pedCnt = pedset.num_pedestrians();

  for ( VIPRA::idx idx = 0; idx < pedCnt; ++idx ) {
    if ( goals.is_goal_met(idx) ) continue;

    auto const pedDir = goals.current_goal(idx) - pedset.ped_coords(idx);

    if ( calc_collisions(idx, pedset, goals) ) {
      _statuses[idx] = Status::WON;
    }
    else {
      _statuses[idx] = Status::LOST;
    }
  }
}

auto CollisionDetection::calc_collisions(VIPRA::idx                         pedIdx,
                                         VIPRA::Modules::Pedestrians const& pedset,
                                         VIPRA::Modules::Goals const&       goals) -> bool
{
  auto const neighbors = pedset.all_neighbors_within(pedIdx, 2 * _range);

  for ( VIPRA::idx neighbor : neighbors ) {
    if ( pedIdx == neighbor ) continue;
    if ( goals.is_goal_met(neighbor) ) continue;
    if ( ! VIPRA::Geometry::do_intersect(_collisionRects[pedIdx],
                                         (_collisionRects[neighbor])) )
      continue;

    auto const& pedCoords = pedset.ped_coords(pedIdx);
    auto const& neighborCoords = pedset.ped_coords(neighbor);
    auto const  pedDir = goals.current_goal(pedIdx) - pedCoords;
    auto const  neighborDir = goals.current_goal(neighbor) - neighborCoords;
    auto const  dotProduct = pedDir.dot(neighborDir);

    if ( dotProduct < 0 || dotProduct > 0.9 ) continue;

    auto const midpoint = calc_intersection_midpoint(pedIdx, neighbor);

    if ( pedCoords.distance_to_sqrd(midpoint) >
         neighborCoords.distance_to_sqrd(midpoint) )
      return false;
  }

  return true;
}

void CollisionDetection::calc_collision_shapes(VIPRA::Modules::Pedestrians const& pedset,
                                               CALM::ModelData const& modelData)
{
  auto const&       coords = pedset.all_coords();
  auto const&       velocities = pedset.all_velocities();
  auto const&       shldrs = modelData.shoulders;
  VIPRA::size const pedCnt = pedset.num_pedestrians();

  for ( VIPRA::idx idx = 0; idx < pedCnt; ++idx ) {
    VIPRA::f3d const& pedVel = velocities[idx];
    VIPRA::f3d const  range = (pedVel.unit() * _range);

    _collisionRects[idx] =
        VIPRA::Geometry::Rectangle(shldrs[idx].start, shldrs[idx].start + range,
                                   shldrs[idx].end + range, shldrs[idx].end);
  }
}

inline auto CollisionDetection::calc_intersection_midpoint(
    VIPRA::idx index1, VIPRA::idx index2) -> VIPRA::f3d
{
  auto const& rect1 = _collisionRects[index1];
  auto const& rect2 = _collisionRects[index2];
  auto const  sides1 = rect1.sides();
  auto const  sides2 = rect2.sides();

  VIPRA::f3dVec intersectionPoints;

  for ( auto const& point : rect1.points() ) {
    if ( rect2.is_point_inside(point) ) intersectionPoints.emplace_back(point);
  }

  for ( auto const& point : rect2.points() ) {
    if ( rect1.is_point_inside(point) ) intersectionPoints.emplace_back(point);
  }

  for ( auto const& side1 : sides1 ) {
    for ( auto const& side2 : sides2 ) {
      if ( side1.does_intersect(side2) ) {
        intersectionPoints.emplace_back(side1.intersection_point(side2));
      }
    }
  }

  VIPRA::f3d midpoint(0, 0, 0);
  for ( auto const& intersectionPoint : intersectionPoints ) {
    midpoint += intersectionPoint;
  }
  midpoint /= intersectionPoints.size();

  return midpoint;
}

auto CollisionDetection::status(VIPRA::idx idx) -> CollisionDetection::Status
{
  return _statuses[idx];
}
}  // namespace CALM