#pragma once

#include "vipra/concepts/goals.hpp"
#include "vipra/concepts/map.hpp"
#include "vipra/concepts/pedset.hpp"

#include "calm_model_types.hpp"
#include "vipra/types/rectangle.hpp"

namespace CALM {

class Collision {
public:
  void initialize(const VIPRA::Concepts::PedsetModule auto &pedset,
                  const VIPRA::Concepts::GoalsModule auto &goals,
                  const ModelData &data) {
    raceStatuses = std::vector<RaceStatus>(pedset.num_pedestrians(), NO_RACE);
    inRace = std::vector<std::vector<bool>>(
        pedset.num_pedestrians(),
        std::vector<bool>(pedset.num_pedestrians(), false));

    intersectionMidpoints = std::vector<VIPRA::f3dVec>(
        pedset.num_pedestrians(), VIPRA::f3dVec(pedset.num_pedestrians()));

    velocityDirections = VIPRA::f3dVec(pedset.num_pedestrians());

    initializeRectangles(pedset, goals, data);
  }

  void raceDetection(const VIPRA::Concepts::PedsetModule auto &pedset,
                     const ModelData &data,
                     const VIPRA::Concepts::GoalsModule auto &goals,
                     VIPRA::timestep timestep,
                     const VIPRA::Concepts::MapModule auto &map) {
    calcCollisionRectangles(pedset, goals, data);
    const VIPRA::size pedCnt = pedset.num_pedestrians();

    for (VIPRA::idx i = 0; i < pedCnt; ++i) {
      if (goals.is_goal_met(i))
        continue;

      bool check = checkIfHighestPriority(pedset, goals, i, timestep);
      if (check) {
        raceStatuses.at(i) = NO_RACE;
      } else {
        raceStatuses.at(i) = WAIT;
      }
    }
  }

  void initializeRectangles(const VIPRA::Concepts::PedsetModule auto &pedset,
                            const VIPRA::Concepts::GoalsModule auto &goals,
                            const ModelData &data) {
    collisionRectangles =
        std::vector<VIPRA::Rectangle>(pedset.num_pedestrians());
    const auto &shldrs = data.shoulderLens;
    for (VIPRA::size i = 0; i < collisionRectangles.size(); i++) {
      auto coords = pedset.ped_coords(i);
      auto goalCoords = goals.current_goal(i);
      auto goalDirection = goalCoords - coords;

      auto shldr1 = (VIPRA::f3d{-goalDirection.y, goalDirection.x}.unit() *
                     shldrs.at(i)) +
                    coords;
      auto shldr2 = (VIPRA::f3d{goalDirection.y, -goalDirection.x}.unit() *
                     shldrs.at(i)) +
                    coords;

      const VIPRA::f3d range = (goalDirection.unit() * rectangleRange);
      collisionRectangles[i] = {shldr1, shldr1 + range, shldr2 + range, shldr2};
    }
  }

  void assignRaceStatuses(std::vector<RaceStatus> &cpmRaceStatuses,
                          std::vector<std::vector<bool>> &cpmInRace) {
    cpmRaceStatuses = raceStatuses;
    cpmInRace = inRace;
  }

  [[nodiscard]] RaceStatus status(VIPRA::idx pedIdx) const {
    return raceStatuses.at(pedIdx);
  }

private:
  std::vector<RaceStatus> raceStatuses;
  std::vector<VIPRA::Rectangle> collisionRectangles;
  std::vector<std::vector<bool>> inRace;
  std::vector<std::vector<VIPRA::f3d>> intersectionMidpoints;
  std::vector<VIPRA::f3d> velocityDirections;

  static constexpr VIPRA::size maxCount = 500;
  static constexpr float minspeed = 0.00000001F;
  static constexpr float rectangleRange = 0.4;

  void calcCollisionRectangles(const VIPRA::Concepts::PedsetModule auto &pedset,
                               const VIPRA::Concepts::GoalsModule auto &goals,
                               const ModelData &data) {
    const auto &coords = pedset.all_coords();
    const auto &velocities = pedset.all_velocities();
    const auto &shldrs = data.shoulderLens;
    const VIPRA::size pedCnt = pedset.num_pedestrians();

    for (VIPRA::idx i = 0; i < pedCnt; ++i) {
      VIPRA::f3d pedVel = velocities.at(i);
      if (pedVel.mag_sqrd() == 0) {
        auto goalCoords = goals.current_goal(i);
        auto goalDirection = goalCoords - coords.at(i);

        auto shldr1 = (VIPRA::f3d{-goalDirection.y, goalDirection.x}.unit() *
                       shldrs.at(i)) +
                      coords.at(i);
        auto shldr2 = (VIPRA::f3d{goalDirection.y, -goalDirection.x}.unit() *
                       shldrs.at(i)) +
                      coords.at(i);

        const VIPRA::f3d range = (goalDirection.unit() * rectangleRange);
        collisionRectangles[i] = {shldr1, shldr1 + range, shldr2 + range,
                                  shldr2};
        continue;
      }

      const VIPRA::Line shoulders =
          getShoulderPoints(coords.at(i), pedVel, shldrs.at(i));
      const VIPRA::f3d range = (pedVel.unit() * rectangleRange);
      collisionRectangles.at(i) = {shoulders.start, shoulders.start + range,
                                   shoulders.end + range, shoulders.end};
    }
  }

  static void addIntersectionPoints(VIPRA::f3d p1, VIPRA::f3d q1, VIPRA::f3d p2,
                                    VIPRA::f3d q2,
                                    VIPRA::f3dVec &intersectionPoints) {
    float a1 = q1.y - p1.y;
    float b1 = p1.x - q1.x;
    float a2 = q2.y - p2.y;
    float b2 = p2.x - q2.x;
    float c1 = a1 * p1.x + b1 * p1.y;
    float c2 = a2 * p2.x + b2 * p2.y;

    float det = a1 * b2 - a2 * b1;

    if (det == 0) {
      if (VIPRA::Line{p1, q1}.is_point_on(p2))
        intersectionPoints.push_back(p2);
      if (VIPRA::Line{p1, q1}.is_point_on(q2))
        intersectionPoints.push_back(q2);
      if (VIPRA::Line{p2, q2}.is_point_on(p1))
        intersectionPoints.push_back(p1);
      if (VIPRA::Line{p2, q2}.is_point_on(q1))
        intersectionPoints.push_back(q1);
    } else {
      float pX{};
      float pY{};
      pX = (b2 * c1 - b1 * c2) / det;
      pY = -(a2 * c1 - a1 * c2) / det;
      intersectionPoints.emplace_back(pX, pY);
    }
  }

  [[nodiscard]] VIPRA::f3d getCollisionAreaMidpoint(VIPRA::idx index1,
                                                    VIPRA::idx index2) {
    const VIPRA::Rectangle &r1 = collisionRectangles.at(index1);
    const VIPRA::Rectangle &r2 = collisionRectangles.at(index2);

    std::array<VIPRA::Line, 4> s1{VIPRA::Line{r1.p1(), r1.p2()},
                                  {r1.p2(), r1.p3()},
                                  {r1.p3(), r1.p4()},
                                  {r1.p4(), r1.p1()}};
    std::array<VIPRA::Line, 4> s2{VIPRA::Line{r2.p1(), r2.p2()},
                                  {r2.p2(), r2.p3()},
                                  {r2.p3(), r2.p4()},
                                  {r2.p4(), r2.p1()}};

    VIPRA::f3dVec intersectionPoints;
    for (size_t i = 0; i < 4; i++) {
      for (size_t j = 0; j < 4; j++) {
        if (s1.at(i).does_intersect(s2.at(j))) {
          addIntersectionPoints(s1.at(i).start, s1.at(i).end, s2.at(j).start,
                                s2.at(j).end, intersectionPoints);
        }
      }
    }

    VIPRA::f3d midpoint(0, 0, 0);
    for (const auto &intersectionPoint : intersectionPoints) {
      midpoint += intersectionPoint;
    }
    midpoint /= intersectionPoints.size();

    return midpoint;
  }

  [[nodiscard]] bool
  checkIfHighestPriority(const VIPRA::Concepts::PedsetModule auto &pedset,
                         const VIPRA::Concepts::GoalsModule auto &goals,
                         VIPRA::idx pedIdx, VIPRA::timestep timestep) {
    bool flag = true;
    for (VIPRA::idx i = 0; i < pedset.num_pedestrians(); i++) {
      if (i == pedIdx)
        continue;
      if (goals.is_goal_met(i))
        continue;

      auto coords1 = pedset.ped_coords(pedIdx);
      auto coords2 = pedset.ped_coords(i);

      constexpr float dist2 = 2 * rectangleRange;
      if (coords1.distance_to(coords2) >= dist2)
        continue;

      bool cr2in1 = collisionRectangles.at(pedIdx).is_point_inside(coords2);
      bool cr1in2 = collisionRectangles.at(i).is_point_inside(coords1);

      auto goal1 = goals.current_goal(pedIdx);
      auto goal2 = goals.current_goal(i);

      // If goals don't match, check if coordinates of the pedestrians are in
      // each others collision rectangles
      if (goal1 != goal2) {
        if (!cr2in1) {
          inRace.at(pedIdx).at(i) = false;
          inRace.at(i).at(pedIdx) = false;
          continue;
        }

        if (!cr1in2) {
          inRace.at(i).at(pedIdx) = true;
          inRace.at(pedIdx).at(i) = true;
          flag = false;
          continue;
        }
      }

      // Check if pedestrians collide, if not continue onto next passenger
      bool collisionCheck = checkIfCollide(pedIdx, i);
      if (!collisionCheck) {
        if (inRace.at(pedIdx).at(i)) {
          inRace.at(pedIdx).at(i) = false;
          inRace.at(i).at(pedIdx) = false;
        }
        continue;
      }

      // If goals match, use distance comparision
      if (goal1 == goal2) {
        if (coords1.distance_to(goal1) > coords2.distance_to(goal2)) {
          inRace.at(i).at(pedIdx) = true;
          inRace.at(pedIdx).at(i) = true;
          flag = false;
        } else {
          if (inRace.at(pedIdx).at(i)) {
            inRace.at(pedIdx).at(i) = false;
            inRace.at(i).at(pedIdx) = false;
          }
        }
        continue;
      }

      // If goals don't match, use collision midpoint to resolve race condition
      VIPRA::f3d collisionMidpoint;
      if (!inRace.at(pedIdx).at(i)) {
        collisionMidpoint = getCollisionAreaMidpoint(pedIdx, i);

        velocityDirections.at(pedIdx) = pedset.ped_velocity(pedIdx).unit();

        intersectionMidpoints.at(pedIdx).at(i) = collisionMidpoint;
        intersectionMidpoints.at(i).at(pedIdx) = collisionMidpoint;

        inRace.at(i).at(pedIdx) = true;
        inRace.at(pedIdx).at(i) = true;
      } else {
        collisionMidpoint = intersectionMidpoints.at(pedIdx).at(i);
      }
      if (coords1.distance_to(collisionMidpoint) >
          coords2.distance_to(collisionMidpoint))
        flag = false;
      else if (coords1.distance_to(collisionMidpoint) ==
               coords2.distance_to(collisionMidpoint)) {
        if (pedIdx < i)
          flag = false;
      }
    }
    return flag;
  }

  /**
   * @brief Checks if two indices' collision rectangles collide
   *
   * @param index1 :
   * @param index2 :
   * @return true
   * @return false
   */
  bool checkIfCollide(VIPRA::idx index1, VIPRA::idx index2) {
    VIPRA::Rectangle &r1 = collisionRectangles.at(index1);
    VIPRA::Rectangle &r2 = collisionRectangles.at(index2);
    return r1.does_intersect(r2);
  }

  /**
   * @brief Gets the shoulder points of a pedestrian
   *
   * @param coords : Coordinates of the pedestrian
   * @param velocity : Velocity of the pedestrian
   * @param shoulderWidth : Width of the pedestrian
   * @return VIPRA::Line : Line containing the shoulder points
   */
  [[nodiscard]] inline VIPRA::Line
  getShoulderPoints(const VIPRA::f3d &coords, const VIPRA::f3d &velocity,
                    float shoulderWidth) noexcept {
    if (velocity == VIPRA::f3d{0, 0, 0}) {
      return {(VIPRA::f3d{0, shoulderWidth}), (VIPRA::f3d{0, -shoulderWidth})};
    }
    return {
        (VIPRA::f3d{-velocity.y, velocity.x}.unit() * shoulderWidth) + coords,
        (VIPRA::f3d{velocity.y, -velocity.x}.unit() * shoulderWidth) + coords};
  }
};
} // namespace CALM