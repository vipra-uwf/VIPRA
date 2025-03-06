
#include "Calm.hpp"
#include "vipra/geometry/rectangle.hpp"

VIPRA_REGISTER_MODULE(Model::Calm, Model)

void Model::Calm::calc_shoulders(VIPRA::f3dVec const& coords, VIPRA::f3dVec const& goals)
{
  VIPRA::size pedCnt = coords.size();

  for ( VIPRA::idx i = 0; i < pedCnt; ++i ) {
    VIPRA::f3d coord = coords[i];
    VIPRA::f3d direction = (goals[i] - coord).unit();

    _peds.shoulders[i] = {
        (VIPRA::f3d{-direction.y, direction.x}.unit() * _peds.shoulderLens[i]) + coord,
        (VIPRA::f3d{direction.y, -direction.x}.unit() * _peds.shoulderLens[i]) + coord};
  }
}

auto Model::Calm::obj_spatial_test(const VIPRA::Geometry::Rectangle& collisionRect,
                                   VIPRA::f3d objLeft, VIPRA::f3d objRight) -> bool
{
  auto const& sides = collisionRect.sides();

  if ( sides[0].start == sides[1].start ) {
    return false;
  }

  VIPRA::f3d center = (objLeft + objRight) /= 2;

  return collisionRect.is_point_inside(center) ||
         collisionRect.is_point_inside(objLeft) ||
         collisionRect.is_point_inside(objRight);
}

auto Model::Calm::obj_direction_test(VIPRA::f3d pedCoord, VIPRA::f3d veloc,
                                     VIPRA::f3d objCoords) -> bool
{
  const VIPRA::f3d displacement = objCoords - pedCoord;

  VIPRA::f_pnt const dotProduct = (veloc.x * displacement.x) +
                                  (veloc.y * displacement.y) + (veloc.z * displacement.z);

  return dotProduct > 0;
}

auto Model::Calm::is_ped_toward_goal(VIPRA::f3d pedCoords, VIPRA::f3d goal,
                                     VIPRA::f3d otherCoords) -> bool
{
  VIPRA::f3d pedDirection = goal - pedCoords;
  VIPRA::f3d secondDirection = otherCoords - pedCoords;

  return pedDirection.dot(secondDirection) > 0;
}

auto Model::Calm::rect_from_shoulders(VIPRA::idx pedIdx, VIPRA::f3d pedCoords,
                                      VIPRA::f3d goal) -> VIPRA::Geometry::Rectangle
{
  const VIPRA::Geometry::Line pedShldr = _peds.shoulders[pedIdx];
  const VIPRA::f3d            range = (goal - pedCoords).unit();

  return VIPRA::Geometry::Rectangle{pedShldr.start, pedShldr.start + range,
                                    pedShldr.end + range, pedShldr.end};
}

void Model::Calm::calc_betas()
{
  std::transform(_peds.betas.begin(), _peds.betas.end(), _peds.nearestDists.begin(),
                 _peds.betas.begin(), [](VIPRA::f_pnt /**/, VIPRA::f_pnt distance) {
                   return calc_beta(distance);
                 });
}

void Model::Calm::calc_neighbors(auto const& pedset, auto const& /*map*/,
                                 auto const& goals)
{
  const VIPRA::size pedCnt = pedset.num_pedestrians();
  auto const&       coords = pedset.all_coords();

  for ( VIPRA::idx i = 0; i < pedCnt; ++i ) {
    if ( goals.is_goal_met(i) ) continue;

    VIPRA::f3d                 pedCoords = coords[i];
    VIPRA::f3d                 pedGoal = goals.current_goal(i);
    VIPRA::Geometry::Rectangle pedRect = rect_from_shoulders(i, pedCoords, pedGoal);

    auto nearest = pedset.conditional_closest_ped(i, [&](VIPRA::idx other) {
      if ( i == other || goals.is_goal_met(other) ) return false;

      auto         otherCoords = coords[other];
      VIPRA::f_pnt distance = pedCoords.distance_to(otherCoords);

      if ( (_collision.raceStatuses[i] == 0 && _collision.raceStatuses[other] == 1) ||
           std::fabs(distance - EQUILIBRIUM_DISTANCE) < EQUILIBRIUM_RESOLUTION )
        return false;

      if ( ! is_ped_toward_goal(pedCoords, pedGoal, otherCoords) ) return false;

      if ( ! obj_spatial_test(pedRect, _peds.shoulders[other].start,
                              _peds.shoulders[other].end) )
        return false;

      return true;
    });

    if ( nearest == i ) {
      _peds.nearestDists[i] = std::numeric_limits<VIPRA::f_pnt>::max();
    }
    else {
      _peds.nearestDists[i] = pedCoords.distance_to(coords[nearest]);
    }
  }
}

void Model::Calm::update_state(auto const& pedset, auto const& goals, VIPRA::State& state,
                               VIPRA::delta_t deltaT)
{
  const VIPRA::size pedCnt = pedset.num_pedestrians();
  auto const&       velocities = pedset.all_velocities();
  auto const&       coords = pedset.all_coords();

  for ( VIPRA::idx i = 0; i < pedCnt; ++i ) {
    if ( goals.is_goal_met(i) || _collision.status(i) == CALM::WAIT ) {
      state.velocities[i] = VIPRA::f3d{};
      continue;
    }

    const VIPRA::f3d   goal = goals.current_goal(i);
    const VIPRA::f3d   velocity = velocities[i];
    const VIPRA::f3d   coord = coords[i];
    const VIPRA::f_pnt desiredSpeed = _peds.maxSpeeds[i];
    const VIPRA::f_pnt mass = _peds.masses[i];
    const VIPRA::f_pnt reactionTime = _peds.reactionTimes[i];
    const VIPRA::f_pnt beta = _peds.betas[i];
    const VIPRA::f3d   direction = goal - coord;

    const VIPRA::f3d propulsion =
        ((direction.unit() * desiredSpeed * beta - velocity) * mass) / reactionTime;

    state.velocities[i] = ((propulsion / mass) * deltaT) + velocity;

    if ( goals.time_since_last_goal(i) > 0 &&
         goals.time_since_last_goal(i) <= SLIDING_GOAL_TIME ) {
      state.velocities[i].x = 0;
      state.velocities[i].y = 0;
    }

    state.positions[i] = coord + (state.velocities[i] * deltaT);
  }
}

auto Model::Calm::is_path_blocked(VIPRA::idx pedIdx, VIPRA::f3d velocity,
                                  VIPRA::f_pnt maxDist, auto const& map) -> VIPRA::f_pnt
{
  VIPRA::Geometry::Line shoulders = _peds.shoulders[pedIdx];
  if ( shoulders.start == shoulders.end ) {
    return -1;
  }

  VIPRA::f_pnt const leftDist =
      map.ray_hit(shoulders.start, shoulders.start + (velocity.unit() * maxDist));
  VIPRA::f_pnt const rightDist =
      map.ray_hit(shoulders.end, shoulders.end + (velocity.unit() * maxDist));

  if ( leftDist == -1 ) {
    return rightDist;
  }
  if ( rightDist == -1 ) {
    return leftDist;
  }

  return std::min(leftDist, rightDist);
}