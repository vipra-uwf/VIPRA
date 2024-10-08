#pragma once

#include "vipra/modules/model.hpp"

#include "vipra/macros/model.hpp"
#include "vipra/macros/module.hpp"
#include "vipra/macros/parameters.hpp"
#include "vipra/macros/performance.hpp"

#include "vipra/random/distributions.hpp"

#include "calm_collision.hpp"
#include "calm_model_types.hpp"

namespace Model {
class Calm : public VIPRA::Modules::Module<Calm>, public VIPRA::Modules::Model {
 public:
  VIPRA_MODULE_NAME("Calm")
  VIPRA_MODULE_TYPE(Model)

  VIPRA_REGISTER_PARAMS(
      VIPRA_PARAM("meanMass", _config.meanMass),
      VIPRA_PARAM("massStdDev", _config.massStdDev),
      VIPRA_PARAM("meanMass", _config.meanMass),
      VIPRA_PARAM("massStdDev", _config.massStdDev),
      VIPRA_PARAM("meanReactionTime", _config.meanReactionTime),
      VIPRA_PARAM("reactionTimeStdDev", _config.reactionTimeStdDev),
      VIPRA_PARAM("meanMaxSpeed", _config.meanMaxSpeed),
      VIPRA_PARAM("maxSpeedStdDev", _config.maxSpeedStdDev),
      VIPRA_PARAM("meanShoulderLen", _config.meanShoulderLen),
      VIPRA_PARAM("shoulderLenStdDev", _config.shoulderLenStdDev))

  // NOLINTNEXTLINE(misc-unused-parameters)
  VIPRA_MODEL_INIT_STEP
  {
    _peds.resize(pedset.num_pedestrians());
    _collision.initialize(pedset, goals, _peds);

    _peds.masses =
        VIPRA::Random::make_distribution<VIPRA::Random::normal_distribution<>>(
            {_config.meanMass, _config.massStdDev}, _peds.size(), engine);
    _peds.reactionTimes =
        VIPRA::Random::make_distribution<VIPRA::Random::normal_distribution<>>(
            {_config.meanReactionTime, _config.reactionTimeStdDev},
            _peds.size(), engine);
    _peds.maxSpeeds =
        VIPRA::Random::make_distribution<VIPRA::Random::normal_distribution<>>(
            {_config.meanMaxSpeed, _config.maxSpeedStdDev}, _peds.size(),
            engine);
    _peds.shoulderLens =
        VIPRA::Random::make_distribution<VIPRA::Random::normal_distribution<>>(
            {_config.meanShoulderLen, _config.shoulderLenStdDev}, _peds.size(),
            engine);
  }

  // NOLINTNEXTLINE(bugprone-easily-swappable-parameters, misc-unused-parameters)
  VIPRA_MODEL_TIMESTEP
  {
    calc_shoulders(pedset.all_coords(), goals.current_goals());
    calc_neighbors(pedset, map, goals);
    calc_betas();
    update_state(pedset, goals, state, deltaT);

    if ( timestep > 0 ) {
      _collision.race_detection(pedset, _peds, goals, timestep, map);
    }
  }

 private:
  CALM::ModelData  _peds;
  CALM::ConfigData _config{};
  CALM::Collision  _collision;

  static constexpr VIPRA::delta_t SLIDING_GOAL_TIME = 0.1F;
  static constexpr VIPRA::f_pnt   EQUILIBRIUM_DISTANCE = 0.382;
  static constexpr VIPRA::f_pnt   EQUILIBRIUM_RESOLUTION = 0.01F;

  void calc_neighbors(auto const& pedset, auto const& /*map*/,
                      auto const& goals);
  void update_state(auto const& pedset, auto const& goals, VIPRA::State& state,
                    VIPRA::delta_t deltaT);
  auto is_path_blocked(VIPRA::idx pedIdx, VIPRA::f3d veloc,
                       VIPRA::f_pnt maxDist, auto const& map) -> VIPRA::f_pnt;

  void        calc_shoulders(VIPRA::f3dVec const&, VIPRA::f3dVec const&);
  static auto obj_spatial_test(const VIPRA::Geometry::Rectangle&, VIPRA::f3d,
                               VIPRA::f3d) -> bool;
  static auto obj_direction_test(VIPRA::f3d, VIPRA::f3d, VIPRA::f3d) -> bool;
  static auto is_ped_toward_goal(VIPRA::f3d, VIPRA::f3d, VIPRA::f3d) -> bool;

  auto rect_from_shoulders(VIPRA::idx, VIPRA::f3d,
                           VIPRA::f3d) -> VIPRA::Geometry::Rectangle;

  void                               calc_betas();
  VIPRA_INLINE static constexpr auto calc_beta(VIPRA::f_pnt distance)
      -> VIPRA::f_pnt
  {
    constexpr VIPRA::f_pnt VAL_A = -2.11;
    constexpr VIPRA::f_pnt VAL_B = 0.366;
    constexpr VIPRA::f_pnt VAL_C = 0.966;
    return (VAL_C - std::exp(VAL_A * (distance - VAL_B)));
  }
};

// ------------------- IMPLEMENTATIONS -----------------------------------------------------

void Model::Calm::calc_neighbors(auto const& pedset, auto const& /*map*/,
                                 auto const& goals)
{
  const VIPRA::size pedCnt = pedset.num_pedestrians();
  auto const&       coords = pedset.all_coords();

  for ( VIPRA::idx i = 0; i < pedCnt; ++i ) {
    if ( goals.is_goal_met(i) ) continue;

    VIPRA::f3d                 pedCoords = coords[i];
    VIPRA::f3d                 pedGoal = goals.current_goal(i);
    VIPRA::Geometry::Rectangle pedRect =
        rect_from_shoulders(i, pedCoords, pedGoal);

    auto nearest = pedset.conditional_closest_ped(i, [&](VIPRA::idx other) {
      if ( i == other || goals.is_goal_met(other) ) return false;

      auto         otherCoords = coords[other];
      VIPRA::f_pnt distance = pedCoords.distance_to(otherCoords);

      if ( (_collision.raceStatuses[i] == 0 &&
            _collision.raceStatuses[other] == 1) ||
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

void Model::Calm::update_state(auto const& pedset, auto const& goals,
                               VIPRA::State& state, VIPRA::delta_t deltaT)
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
        ((direction.unit() * desiredSpeed * beta - velocity) * mass) /
        reactionTime;

    state.velocities[i] = ((propulsion / mass) * deltaT) + velocity;
    state.positions[i] = coord + (state.velocities[i] * deltaT);
  }
}

auto Model::Calm::is_path_blocked(VIPRA::idx pedIdx, VIPRA::f3d velocity,
                                  VIPRA::f_pnt maxDist,
                                  auto const&  map) -> VIPRA::f_pnt
{
  VIPRA::Geometry::Line shoulders = _peds.shoulders[pedIdx];
  if ( shoulders.start == shoulders.end ) {
    return -1;
  }

  VIPRA::f_pnt const leftDist = map.ray_hit(
      shoulders.start, shoulders.start + (velocity.unit() * maxDist));
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
}  // namespace Model