
#include "Calm.hpp"

extern "C" auto create_module() -> VIPRA ::Modules ::Model*
{
  return new Model::Calm();
}
extern "C" void setup_module(void* mod, VIPRA ::Parameters& paramIn,
                             VIPRA ::Random ::Engine& engine)
{
  static_cast<Model ::Calm*>(mod)->register_params(paramIn);
  static_cast<Model ::Calm*>(mod)->config(paramIn, engine);
}

void Model::Calm::calc_shoulders(VIPRA::f3dVec const& coords,
                                 VIPRA::f3dVec const& goals)
{
  VIPRA::size pedCnt = coords.size();

  for ( VIPRA::idx i = 0; i < pedCnt; ++i ) {
    VIPRA::f3d coord = coords[i];
    VIPRA::f3d direction = (goals[i] - coord).unit();

    _peds.shoulders[i] = {
        (VIPRA::f3d{-direction.y, direction.x}.unit() * _peds.shoulderLens[i]) +
            coord,
        (VIPRA::f3d{direction.y, -direction.x}.unit() * _peds.shoulderLens[i]) +
            coord};
  }
}

auto Model::Calm::obj_spatial_test(
    const VIPRA::Geometry::Rectangle& collisionRect, VIPRA::f3d objLeft,
    VIPRA::f3d objRight) -> bool
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
                                  (veloc.y * displacement.y) +
                                  (veloc.z * displacement.z);

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
                                      VIPRA::f3d goal)
    -> VIPRA::Geometry::Rectangle
{
  const VIPRA::Geometry::Line pedShldr = _peds.shoulders[pedIdx];
  const VIPRA::f3d            range = (goal - pedCoords).unit();

  return {pedShldr.start, pedShldr.start + range, pedShldr.end + range,
          pedShldr.end};
}

void Model::Calm::calc_betas()
{
  std::transform(_peds.betas.begin(), _peds.betas.end(),
                 _peds.nearestDists.begin(), _peds.betas.begin(),
                 [](VIPRA::f_pnt /**/, VIPRA::f_pnt distance) {
                   return calc_beta(distance);
                 });
}