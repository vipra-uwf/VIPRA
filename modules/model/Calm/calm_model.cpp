
#include "calm_model.hpp"

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

  VIPRA::f3d const center = (objLeft + objRight) /= 2;

  return collisionRect.is_point_inside(center) ||
         collisionRect.is_point_inside(objLeft) ||
         collisionRect.is_point_inside(objRight);
}

auto Model::Calm::obj_direction_test(VIPRA::f3d pedCoord, VIPRA::f3d veloc,
                                     VIPRA::f3d objCoords) -> bool
{
  VIPRA::f3d const displacement = objCoords - pedCoord;

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
  VIPRA::Geometry::Line const pedShldr = _peds.shoulders[pedIdx];
  VIPRA::f3d const            range = (goal - pedCoords).unit();

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

void Model::Calm::calc_neighbors(VIPRA::Modules::Pedestrians const& pedset,
                                 VIPRA::Modules::Goals const&       goals,
                                 VIPRA::Modules::Map const& /*map*/)
{
  VIPRA::size const pedCnt = pedset.num_pedestrians();
  auto const&       coords = pedset.all_coords();

  for ( VIPRA::idx i = 0; i < pedCnt; ++i ) {
    if ( goals.is_goal_met(i) ) continue;
    VIPRA::f3d const                 pedCoords = coords[i];
    VIPRA::f3d const                 pedGoal = goals.current_goal(i);
    VIPRA::Geometry::Rectangle const pedRect = rect_from_shoulders(i, pedCoords, pedGoal);

    auto const nearest = pedset.conditional_closest_ped(i, [&](VIPRA::idx other) {
      if ( i == other || goals.is_goal_met(other) ) return false;

      const auto   otherCoords = coords[other];
      VIPRA::f_pnt distance = pedCoords.distance_to(otherCoords);

      if ( ! is_ped_toward_goal(pedCoords, pedGoal, otherCoords) ) return false;

      if ( ! obj_spatial_test(pedRect, _peds.shoulders[other].start,
                              _peds.shoulders[other].end) )
        return false;

      return true;
    });

    if ( nearest == i ) {
      _peds.nearestDists[i] = 100.0F;
    }
    else {
      _peds.nearestDists[i] = pedCoords.distance_to(coords[nearest]);
    }
  }
}

void Model::Calm::update_state(VIPRA::Modules::Pedestrians const& pedset,
                               VIPRA::Modules::Goals const& goals, VIPRA::State& state,
                               VIPRA::delta_t deltaT)
{
  VIPRA::size const pedCnt = pedset.num_pedestrians();
  auto const&       velocities = pedset.all_velocities();
  auto const&       coords = pedset.all_coords();

  for ( VIPRA::idx i = 0; i < pedCnt; ++i ) {
    if ( goals.is_goal_met(i) ) {
      state.velocities[i] = VIPRA::f3d{};
      continue;
    }

    if ( _collision.status(i) == CALM::CollisionDetection::Status::LOST ) {
      state.velocities[i] = VIPRA::f3d{};
      continue;
    }

    VIPRA::f3d const   goal = goals.current_goal(i);
    VIPRA::f3d const   velocity = velocities[i];
    VIPRA::f3d const   coord = coords[i];
    VIPRA::f_pnt const desiredSpeed = _peds.maxSpeeds[i];
    VIPRA::f_pnt const mass = _peds.masses[i];
    VIPRA::f_pnt const reactionTime = _peds.reactionTimes[i];
    VIPRA::f_pnt const beta = _peds.betas[i];
    VIPRA::f3d const   direction = (goal - coord).unit();

    VIPRA::f3d const friction =
        _config.frictionCoef * (velocity - ((velocity.dot(direction)) * direction));
    VIPRA::f3d const propulsion =
        (((direction * desiredSpeed * beta - velocity) * mass) / reactionTime) - friction;

    state.velocities[i] = ((propulsion / mass) * deltaT) + velocity;
    state.positions[i] = coord + (state.velocities[i] * deltaT);
  }
}

auto Model::Calm::is_path_blocked(VIPRA::idx pedIdx, VIPRA::f3d velocity,
                                  VIPRA::f_pnt               maxDist,
                                  VIPRA::Modules::Map const& map) -> VIPRA::f_pnt
{
  VIPRA::Geometry::Line const shoulders = _peds.shoulders[pedIdx];
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