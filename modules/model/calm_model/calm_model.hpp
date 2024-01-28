#pragma once

#include "vipra.hpp"

#include "calm_collision.hpp"
#include "calm_model_types.hpp"
#include "vipra/concepts/goals.hpp"

namespace CALM {
class Model {
public:
  void initialize(const VIPRA::Concepts::PedsetModule auto &pedset,
                  const VIPRA::Concepts::MapModule auto & /*unused*/,
                  const VIPRA::Concepts::GoalsModule auto &goals) {
    _peds.resize(pedset.num_pedestrians());
    _state.initialize(pedset);
    _collision.initialize(pedset, goals, _peds);
    _collision.assignRaceStatuses(_raceStatuses, _inRace);

    // TODO(rolland): add proper randomization when we have a random module
    for (VIPRA::idx i = 0; i < _peds.masses.size(); ++i) {
      _peds.masses[i] = _config.meanMass;
      _peds.reactionTimes[i] = _config.meanReactionTime;
      _peds.maxSpeeds[i] = _config.meanMaxSpeed;
      _peds.shoulderLens[i] = _config.meanShoulderLen;
    }
  }

  template <VIPRA::Concepts::PedsetModule peds_t,
            VIPRA::Concepts::MapModule map_t,
            VIPRA::Concepts::GoalsModule goals_t>
  auto timestep(const peds_t &pedset, const map_t &map, const goals_t &goals,
                VIPRA::delta_t deltaT, VIPRA::timestep timestep)
      -> const VIPRA::State & {
    calc_shoulders(pedset, goals);
    calc_neighbors(pedset, map, goals);
    calc_betas(pedset);
    update_state(pedset, goals, deltaT);

    if (timestep > 0) {
      _collision.raceDetection(pedset, _peds, goals, timestep, map);
      _collision.assignRaceStatuses(_raceStatuses, _inRace);
    }

    return _state;
  }

  VIPRA_MODULE_NAME("calm_model")
  VIPRA_MODULE_TYPE(MODEL)

  VIPRA_REGISTER_STEP {
    VIPRA_REGISTER_PARAM(MODEL, "meanMass", VIPRA::ParameterType::REQUIRED);
    VIPRA_REGISTER_PARAM(MODEL, "massStdDev", VIPRA::ParameterType::REQUIRED);
    VIPRA_REGISTER_PARAM(MODEL, "meanReactionTime",
                         VIPRA::ParameterType::REQUIRED);
    VIPRA_REGISTER_PARAM(MODEL, "reactionTimeStdDev",
                         VIPRA::ParameterType::REQUIRED);
    VIPRA_REGISTER_PARAM(MODEL, "meanMaxSpeed", VIPRA::ParameterType::REQUIRED);
    VIPRA_REGISTER_PARAM(MODEL, "maxSpeedStdDev",
                         VIPRA::ParameterType::REQUIRED);
    VIPRA_REGISTER_PARAM(MODEL, "meanShoulderLen",
                         VIPRA::ParameterType::REQUIRED);
    VIPRA_REGISTER_PARAM(MODEL, "shoulderLenStdDev",
                         VIPRA::ParameterType::REQUIRED);
  }

  VIPRA_CONFIG_STEP {
    _config.meanMass = VIPRA_GET_PARAM(MODEL, "meanMass", VIPRA::f_pnt);
    _config.massStdDev = VIPRA_GET_PARAM(MODEL, "massStdDev", VIPRA::f_pnt);
    _config.meanReactionTime =
        VIPRA_GET_PARAM(MODEL, "meanReactionTime", VIPRA::f_pnt);
    _config.reactionTimeStdDev =
        VIPRA_GET_PARAM(MODEL, "reactionTimeStdDev", VIPRA::f_pnt);
    _config.meanMaxSpeed = VIPRA_GET_PARAM(MODEL, "meanMaxSpeed", VIPRA::f_pnt);
    _config.maxSpeedStdDev =
        VIPRA_GET_PARAM(MODEL, "maxSpeedStdDev", VIPRA::f_pnt);
    _config.meanShoulderLen =
        VIPRA_GET_PARAM(MODEL, "meanShoulderLen", VIPRA::f_pnt);
    _config.shoulderLenStdDev =
        VIPRA_GET_PARAM(MODEL, "shoulderLenStdDev", VIPRA::f_pnt);
  }

private:
  ModelData _peds;
  ConfigData _config;
  VIPRA::State _state;
  CALM::Collision _collision;

  std::vector<RaceStatus> _raceStatuses;
  std::vector<std::vector<bool>> _inRace;

  static constexpr VIPRA::delta_t slidingGoalTime = 0.1F;
  static constexpr float equilibriumDistance = 0.382;
  static constexpr float equilibriumResolution = 0.01F;

  void calc_neighbors(const VIPRA::Concepts::PedsetModule auto &pedset,
                      const VIPRA::Concepts::MapModule auto &map,
                      const VIPRA::Concepts::GoalsModule auto &goals) {
    const VIPRA::size pedCnt = pedset.num_pedestrians();
    const auto &coords = pedset.all_coords();

    for (VIPRA::idx i = 0; i < pedCnt; ++i) {
      if (goals.is_goal_met(i))
        continue;

      VIPRA::f3d pedCoords = coords.at(i);
      VIPRA::f3d pedGoal = goals.current_goal(i);
      VIPRA::Rectangle pedRect = rect_from_shoulders(i, pedCoords, pedGoal);

      VIPRA::f_pnt nearestDist = std::numeric_limits<VIPRA::f_pnt>::max();
      for (VIPRA::idx j = 0; j < pedCnt; ++j) {
        if (i == j || goals.is_goal_met(j))
          continue;

        auto otherCoords = coords.at(j);
        VIPRA::f_pnt distance = pedCoords.distance_to(otherCoords);

        if (distance >= nearestDist)
          continue;

        if ((_raceStatuses.at(i) == 0 && _raceStatuses.at(j) == 1) ||
            std::fabs(distance - equilibriumDistance) < equilibriumResolution)
          continue;

        if (!is_ped_toward_goal(pedCoords, pedGoal, otherCoords))
          continue;

        if (!obj_spatial_test(pedRect, _peds.shoulders.at(j).start,
                              _peds.shoulders.at(j).end))
          continue;

        nearestDist = distance;
      }
      _peds.nearestDists.at(i) = nearestDist;
    }
  }

  void calc_shoulders(const VIPRA::Concepts::PedsetModule auto &pedset,
                      const VIPRA::Concepts::GoalsModule auto &goals) {
    VIPRA::size pedCnt = pedset.num_pedestrians();
    const auto &coords = pedset.all_coords();

    for (VIPRA::idx i = 0; i < pedCnt; ++i) {
      VIPRA::f3d coord = coords.at(i);
      VIPRA::f3d direction = (goals.current_goal(i) - coord).unit();

      _peds.shoulders.at(i) = {(VIPRA::f3d{-direction.y, direction.x}.unit() *
                                _peds.shoulderLens.at(i)) +
                                   coord,
                               (VIPRA::f3d{direction.y, -direction.x}.unit() *
                                _peds.shoulderLens.at(i)) +
                                   coord};
    }
  }

  void update_state(const VIPRA::Concepts::PedsetModule auto &pedset,
                    const VIPRA::Concepts::GoalsModule auto &goals,
                    VIPRA::delta_t dT) {
    const VIPRA::size pedCnt = pedset.num_pedestrians();
    const auto &velocities = pedset.all_velocities();
    const auto &coords = pedset.all_coords();

    for (VIPRA::idx i = 0; i < pedCnt; ++i) {
      if (goals.is_goal_met(i) || _collision.status(i) == WAIT) {
        _state.velocities[i] = VIPRA::f3d{};
        continue;
      }

      const VIPRA::f3d goal = goals.current_goal(i);
      const VIPRA::f3d velocity = velocities[i];
      const VIPRA::f3d coord = coords[i];
      const float desiredSpeed = _peds.maxSpeeds[i];
      const float mass = _peds.masses[i];
      const float reactionTime = _peds.reactionTimes[i];
      const float beta = _peds.betas[i];
      const VIPRA::f3d direction = goal - coord;

      const VIPRA::f3d propulsion =
          ((direction.unit() * desiredSpeed * beta - velocity) * mass) /
          reactionTime;

      _state.velocities[i] = ((propulsion / mass) * dT) + velocity;
      _state.positions[i] = coord + (_state.velocities[i] * dT);
    }
  }

  bool obj_spatial_test(const VIPRA::Rectangle &collisionRect,
                        VIPRA::f3d objLeft, VIPRA::f3d objRight) {
    if (collisionRect.p1() == collisionRect.p2()) {
      return false;
    }

    VIPRA::f3d center = (objLeft + objRight) /= 2;

    return collisionRect.is_point_inside(objLeft) ||
           collisionRect.is_point_inside(center) ||
           collisionRect.is_point_inside(objRight);
  }

  bool obj_direction_test(VIPRA::f3d pedCoord, VIPRA::f3d veloc,
                          VIPRA::f3d objCoords) {
    const VIPRA::f3d displacement = objCoords - pedCoord;

    const float dotProduct = (veloc.x * displacement.x) +
                             (veloc.y * displacement.y) +
                             (veloc.z * displacement.z);

    return dotProduct > 0;
  }

  bool is_ped_toward_goal(VIPRA::f3d pedCoords, VIPRA::f3d goal,
                          VIPRA::f3d otherCoords) {
    VIPRA::f3d pedDirection = goal - pedCoords;
    VIPRA::f3d secondDirection = otherCoords - pedCoords;

    return pedDirection.dot(secondDirection) > 0;
  }

  auto is_path_blocked(VIPRA::idx pedIdx, VIPRA::f3d velocity,
                       VIPRA::f_pnt maxDist,
                       const VIPRA::Concepts::MapModule auto &map) -> float {
    VIPRA::Line shoulders = _peds.shoulders.at(pedIdx);
    if (shoulders.start == shoulders.end) {
      return -1;
    }

    const float leftDist = map.ray_hit(
        shoulders.start, shoulders.start + (velocity.unit() * maxDist));
    const float rightDist =
        map.ray_hit(shoulders.end, shoulders.end + (velocity.unit() * maxDist));

    if (leftDist == -1) {
      return rightDist;
    }
    if (rightDist == -1) {
      return leftDist;
    }

    return std::min(leftDist, rightDist);
  }

  auto rect_from_shoulders(VIPRA::idx pedIdx, VIPRA::f3d pedCoords,
                           VIPRA::f3d goal) -> VIPRA::Rectangle {
    const VIPRA::Line pedShldr = _peds.shoulders.at(pedIdx);
    const VIPRA::f3d range = (goal - pedCoords).unit();

    return VIPRA::Rectangle{pedShldr.start, pedShldr.start + range,
                            pedShldr.end + range, pedShldr.end};
  }

  void calc_betas(const VIPRA::Concepts::PedsetModule auto &pedset) {
    const VIPRA::size pedCnt = pedset.num_pedestrians();

    for (VIPRA::idx i = 0; i < pedCnt; ++i) {
      _peds.betas[i] = calc_beta(_peds.nearestDists[i]);
    }
  }

  constexpr auto calc_beta(VIPRA::f_pnt distance) -> float {
    constexpr float valA = -2.11;
    constexpr float valB = 0.366;
    constexpr float valC = 0.966;
    return (valC - std::exp(valA * (distance - valB)));
  }
};
} // namespace CALM

CHECK_MODULE(ModelModule, CALM::Model)