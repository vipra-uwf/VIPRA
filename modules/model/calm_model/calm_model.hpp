#pragma once

#include "vipra.hpp"

#include "calm_collision.hpp"
#include "calm_model_types.hpp"
#include "vipra/concepts/goals.hpp"
#include "vipra/geometry/f3d.hpp"
#include "vipra/random/distributions.hpp"

namespace CALM {
class Model {
 public:
  VIPRA_MODULE_NAME("calm_model")
  VIPRA_MODULE_TYPE(MODEL)

  VIPRA_REGISTER_STEP {
    VIPRA_REGISTER_PARAM("meanMass", REQUIRED);
    VIPRA_REGISTER_PARAM("massStdDev", REQUIRED);
    VIPRA_REGISTER_PARAM("meanReactionTime", REQUIRED);
    VIPRA_REGISTER_PARAM("reactionTimeStdDev", REQUIRED);
    VIPRA_REGISTER_PARAM("meanMaxSpeed", REQUIRED);
    VIPRA_REGISTER_PARAM("maxSpeedStdDev", REQUIRED);
    VIPRA_REGISTER_PARAM("meanShoulderLen", REQUIRED);
    VIPRA_REGISTER_PARAM("shoulderLenStdDev", REQUIRED);
    VIPRA_REGISTER_PARAM("random_seed", REQUIRED);
  }

  VIPRA_CONFIG_STEP {
    VIPRA_GET_PARAM("meanMass", _config.meanMass);
    VIPRA_GET_PARAM("massStdDev", _config.massStdDev);
    VIPRA_GET_PARAM("meanReactionTime", _config.meanReactionTime);
    VIPRA_GET_PARAM("reactionTimeStdDev", _config.reactionTimeStdDev);
    VIPRA_GET_PARAM("meanMaxSpeed", _config.meanMaxSpeed);
    VIPRA_GET_PARAM("maxSpeedStdDev", _config.maxSpeedStdDev);
    VIPRA_GET_PARAM("meanShoulderLen", _config.meanShoulderLen);
    VIPRA_GET_PARAM("shoulderLenStdDev", _config.shoulderLenStdDev);
    VIPRA_GET_PARAM("random_seed", _config.randomSeed);
  }

  // NOLINTNEXTLINE(misc-unused-parameters)
  VIPRA_MODEL_INIT_STEP {
    _peds.resize(pedset.num_pedestrians());
    _state.initialize(pedset);
    _collision.initialize(pedset, goals, _peds);
    _collision.assignRaceStatuses(_raceStatuses, _inRace);

    VIPRA::Random::Engine engine{_config.randomSeed};

    _peds.masses = VIPRA::make_distribution<VIPRA::normal_distribution<>>(
        {_config.meanMass, _config.massStdDev}, _peds.size(), engine);
    _peds.reactionTimes = VIPRA::make_distribution<VIPRA::normal_distribution<>>(
        {_config.meanReactionTime, _config.reactionTimeStdDev}, _peds.size(), engine);
    _peds.maxSpeeds = VIPRA::make_distribution<VIPRA::normal_distribution<>>(
        {_config.meanMaxSpeed, _config.maxSpeedStdDev}, _peds.size(), engine);
    _peds.shoulderLens = VIPRA::make_distribution<VIPRA::normal_distribution<>>(
        {_config.meanShoulderLen, _config.shoulderLenStdDev}, _peds.size(), engine);
  }

  // NOLINTNEXTLINE(misc-unused-parameters, bugprone-easily-swappable-parameters)
  VIPRA_MODEL_TIMESTEP {
    calc_shoulders(pedset.all_coords(), goals.current_goals());
    calc_neighbors(pedset, map, goals);
    calc_betas();
    update_state(pedset, goals, deltaT);

    if (timestep > 0) {
      _collision.raceDetection(pedset, _peds, goals, timestep, map);
      _collision.assignRaceStatuses(_raceStatuses, _inRace);
    }

    return _state;
  }

 private:
  ModelData       _peds;
  ConfigData      _config;
  VIPRA::State    _state;
  CALM::Collision _collision;

  std::vector<RaceStatus>        _raceStatuses;
  std::vector<std::vector<bool>> _inRace;

  static constexpr VIPRA::delta_t SLIDING_GOAL_TIME = 0.1F;
  static constexpr VIPRA::f_pnt   EQUILIBRIUM_DISTANCE = 0.382;
  static constexpr VIPRA::f_pnt   EQUILIBRIUM_RESOLUTION = 0.01F;

  void calc_neighbors(VIPRA::Concepts::PedsetModule auto const& pedset,
                      VIPRA::Concepts::MapModule auto const& /*map*/,
                      VIPRA::Concepts::GoalsModule auto const& goals);
  void update_state(VIPRA::Concepts::PedsetModule auto const& pedset,
                    VIPRA::Concepts::GoalsModule auto const& goals, VIPRA::delta_t deltaT);
  auto is_path_blocked(VIPRA::idx pedIdx, VIPRA::f3d veloc, VIPRA::f_pnt maxDist,
                       const VIPRA::Concepts::MapModule auto& map) -> VIPRA::f_pnt;

  void        calc_shoulders(VIPRA::f3dVec const&, VIPRA::f3dVec const&);
  static auto obj_spatial_test(const VIPRA::Geometry::Rectangle&, VIPRA::f3d, VIPRA::f3d) -> bool;
  static auto obj_direction_test(VIPRA::f3d, VIPRA::f3d, VIPRA::f3d) -> bool;
  static auto is_ped_toward_goal(VIPRA::f3d, VIPRA::f3d, VIPRA::f3d) -> bool;

  auto rect_from_shoulders(VIPRA::idx, VIPRA::f3d, VIPRA::f3d) -> VIPRA::Geometry::Rectangle;

  void                  calc_betas();
  static constexpr auto calc_beta(VIPRA::f_pnt distance) -> VIPRA::f_pnt;
};

// ------------------- IMPLEMENTATIONS -----------------------------------------------------

void Model::calc_neighbors(VIPRA::Concepts::PedsetModule auto const& pedset,
                           VIPRA::Concepts::MapModule auto const& /*map*/,
                           VIPRA::Concepts::GoalsModule auto const& goals) {
  const VIPRA::size pedCnt = pedset.num_pedestrians();
  auto const&       coords = pedset.all_coords();

  for (VIPRA::idx i = 0; i < pedCnt; ++i) {
    if (goals.is_goal_met(i)) continue;

    VIPRA::f3d                 pedCoords = coords[i];
    VIPRA::f3d                 pedGoal = goals.current_goal(i);
    VIPRA::Geometry::Rectangle pedRect = rect_from_shoulders(i, pedCoords, pedGoal);

    VIPRA::f_pnt nearestDist = std::numeric_limits<VIPRA::f_pnt>::max();
    for (VIPRA::idx j = 0; j < pedCnt; ++j) {
      if (i == j || goals.is_goal_met(j)) continue;

      auto         otherCoords = coords[j];
      VIPRA::f_pnt distance = pedCoords.distance_to(otherCoords);

      if (distance >= nearestDist) continue;

      if ((_raceStatuses[i] == 0 && _raceStatuses[j] == 1) ||
          std::fabs(distance - EQUILIBRIUM_DISTANCE) < EQUILIBRIUM_RESOLUTION)
        continue;

      if (!is_ped_toward_goal(pedCoords, pedGoal, otherCoords)) continue;

      if (!obj_spatial_test(pedRect, _peds.shoulders[j].start, _peds.shoulders[j].end)) continue;

      nearestDist = distance;
    }
    _peds.nearestDists[i] = nearestDist;
  }
}

void Model::update_state(VIPRA::Concepts::PedsetModule auto const& pedset,
                         VIPRA::Concepts::GoalsModule auto const& goals, VIPRA::delta_t deltaT) {
  const VIPRA::size pedCnt = pedset.num_pedestrians();
  auto const&       velocities = pedset.all_velocities();
  auto const&       coords = pedset.all_coords();

  for (VIPRA::idx i = 0; i < pedCnt; ++i) {
    if (goals.is_goal_met(i) || _collision.status(i) == WAIT) {
      _state.velocities[i] = VIPRA::f3d{};
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

    const VIPRA::f3d propulsion = ((direction.unit() * desiredSpeed * beta - velocity) * mass) / reactionTime;

    _state.velocities[i] = ((propulsion / mass) * deltaT) + velocity;
    _state.positions[i] = coord + (_state.velocities[i] * deltaT);
  }
}

auto Model::is_path_blocked(VIPRA::idx pedIdx, VIPRA::f3d velocity, VIPRA::f_pnt maxDist,
                            const VIPRA::Concepts::MapModule auto& map) -> VIPRA::f_pnt {
  VIPRA::Geometry::Line shoulders = _peds.shoulders[pedIdx];
  if (shoulders.start == shoulders.end) {
    return -1;
  }

  VIPRA::f_pnt const leftDist = map.ray_hit(shoulders.start, shoulders.start + (velocity.unit() * maxDist));
  VIPRA::f_pnt const rightDist = map.ray_hit(shoulders.end, shoulders.end + (velocity.unit() * maxDist));

  if (leftDist == -1) {
    return rightDist;
  }
  if (rightDist == -1) {
    return leftDist;
  }

  return std::min(leftDist, rightDist);
}

}  // namespace CALM

CHECK_MODULE(ModelModule, CALM::Model)