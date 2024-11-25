
#include <stdexcept>
#include "vipra/macros/module.hpp"
#include "vipra/macros/parameters.hpp"
#include "vipra/modules/collision_avoidance.hpp"
#include "vipra/types/float.hpp"

NEW_VIPRA_MODULE(ORCA, CollisionAvoidance)
{
 public:
  VIPRA_MODULE_NAME("ORCA");
  VIPRA_MODULE_TYPE(CollisionAvoidance);

  VIPRA_REGISTER_PARAMS(VIPRA_PARAM("radius", _radius),
                        VIPRA_PARAM("time_horizon", _timeHorizon),
                        VIPRA_PARAM("maxSpeed", _maxSpeed));

  VIPRA_COLLISION_INIT_STEP { _invTimeHorizon = 1.0 / _timeHorizon; }

  VIPRA_COLLISION_UPDATE
  {
    for ( VIPRA::idx pedIdx = 0; pedIdx < pedset.num_pedestrians(); ++pedIdx ) {
      if ( ! goals.is_goal_met(pedIdx) ) {
        state.velocities[pedIdx] =
            compute_new_velocity(pedIdx, state, pedset, deltaT, map);

        state.positions[pedIdx] =
            pedset.ped_coords(pedIdx) + (state.velocities[pedIdx] * deltaT);
      }
    }
  }

 private:
  VIPRA::f_pnt _radius;
  VIPRA::f_pnt _maxSpeed;
  VIPRA::f_pnt _timeHorizon;

  VIPRA::f_pnt _invTimeHorizon;

  [[nodiscard]] auto compute_new_velocity(
      VIPRA::idx                         pedIdx, VIPRA::State & state,
      VIPRA::Modules::Pedestrians const& pedset, VIPRA::delta_t timestep,
      VIPRA::Modules::Map const& map) -> VIPRA::f3d;

  void pedestrian_orca_lines(
      VIPRA::idx pedIdx, VIPRA::Modules::Pedestrians const& pedset,
      VIPRA::Modules::Map const& map, VIPRA::delta_t timeStep,
      std::vector<VIPRA::Geometry::Line>& orcaLines) const;
  void obstacle_orca_lines(
      VIPRA::idx pedIdx, VIPRA::Modules::Pedestrians const& pedset,
      VIPRA::Modules::Map const& map, VIPRA::delta_t timeStep,
      std::vector<VIPRA::Geometry::Line>& orcaLines) const;
};