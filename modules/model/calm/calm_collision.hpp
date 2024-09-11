#pragma once

#include "calm_model_types.hpp"
#include "vipra/geometry/rectangle.hpp"
#include "vipra/macros/performance.hpp"

#include "vipra/types/idx.hpp"
#include "vipra/types/time.hpp"

namespace CALM {

using VIPRA::Geometry::Line;
using VIPRA::Geometry::Rectangle;

class Collision {
 public:
  void initialize(auto const& pedset, auto const& goals, ModelData const& data)
  {
    raceStatuses = std::vector<RaceStatus>(pedset.num_pedestrians(), NO_RACE);
    inRace = std::vector<std::vector<bool>>(pedset.num_pedestrians(),
                                            std::vector<bool>(pedset.num_pedestrians(), false));

    intersectionMidpoints =
        std::vector<VIPRA::f3dVec>(pedset.num_pedestrians(), VIPRA::f3dVec(pedset.num_pedestrians()));

    velocityDirections = VIPRA::f3dVec(pedset.num_pedestrians());

    initialize_rectangles(pedset, goals, data);
  }

  VIPRA_PERF_FUNC void race_detection(auto const& pedset, ModelData const& data, auto const& goals,
                                      VIPRA::timestep timestep, auto const& map)
  {
    calc_collision_rectangles(pedset, goals, data);
    const VIPRA::size pedCnt = pedset.num_pedestrians();

    for ( VIPRA::idx i = 0; i < pedCnt; ++i ) {
      if ( goals.is_goal_met(i) ) continue;

      bool check = check_if_highest_priority(pedset, goals, i, timestep);
      if ( check ) {
        raceStatuses[i] = NO_RACE;
      }
      else {
        raceStatuses[i] = WAIT;
      }
    }
  }

  void initialize_rectangles(auto const& pedset, auto const& goals, ModelData const& data)
  {
    collisionRectangles = std::vector<Rectangle>(pedset.num_pedestrians());
    auto const& shldrs = data.shoulderLens;
    for ( VIPRA::size i = 0; i < collisionRectangles.size(); i++ ) {
      auto coords = pedset.ped_coords(i);
      auto goalCoords = goals.current_goal(i);
      auto goalDirection = goalCoords - coords;

      auto shldr1 = (VIPRA::f3d{-goalDirection.y, goalDirection.x}.unit() * shldrs[i]) + coords;
      auto shldr2 = (VIPRA::f3d{goalDirection.y, -goalDirection.x}.unit() * shldrs[i]) + coords;

      const VIPRA::f3d range = (goalDirection.unit() * RECTANGLE_RANGE);
      collisionRectangles[i] = {shldr1, shldr1 + range, shldr2 + range, shldr2};
    }
  }

  [[nodiscard]] auto status(VIPRA::idx pedIdx) const -> RaceStatus { return raceStatuses[pedIdx]; }

  std::vector<RaceStatus>              raceStatuses;
  std::vector<Rectangle>               collisionRectangles;
  std::vector<std::vector<bool>>       inRace;
  std::vector<std::vector<VIPRA::f3d>> intersectionMidpoints;
  std::vector<VIPRA::f3d>              velocityDirections;

  static constexpr VIPRA::size MAX_COUNT = 500;
  static constexpr float       MINSPEED = 0.00000001F;
  static constexpr float       RECTANGLE_RANGE = 0.4;

  VIPRA_PERF_FUNC void calc_collision_rectangles(auto const& pedset, auto const& goals, ModelData const& data)
  {
    auto const&       coords = pedset.all_coords();
    auto const&       velocities = pedset.all_velocities();
    auto const&       shldrs = data.shoulderLens;
    const VIPRA::size pedCnt = pedset.num_pedestrians();

    for ( VIPRA::idx i = 0; i < pedCnt; ++i ) {
      VIPRA::f3d pedVel = velocities[i];

      if ( pedVel == VIPRA::f3d{} ) {
        auto goalCoords = goals.current_goal(i);
        auto goalDirection = goalCoords - coords[i];

        auto shldr1 = (VIPRA::f3d{-goalDirection.y, goalDirection.x}.unit() * shldrs[i]) + coords[i];
        auto shldr2 = (VIPRA::f3d{goalDirection.y, -goalDirection.x}.unit() * shldrs[i]) + coords[i];

        const VIPRA::f3d range = (goalDirection.unit() * RECTANGLE_RANGE);
        collisionRectangles[i] = {shldr1, shldr1 + range, shldr2 + range, shldr2};
        continue;
      }

      const Line       shoulders = get_shoulder_points(coords[i], pedVel, shldrs[i]);
      const VIPRA::f3d range = (pedVel.unit() * RECTANGLE_RANGE);
      collisionRectangles[i] = {shoulders.start, shoulders.start + range, shoulders.end + range,
                                shoulders.end};
    }
  }

  VIPRA_PERF_FUNC static void add_intersection_points(VIPRA::f3d p1, VIPRA::f3d q1, VIPRA::f3d p2,
                                                      VIPRA::f3d q2, VIPRA::f3dVec& intersectionPoints)
  {
    float a1 = q1.y - p1.y;
    float b1 = p1.x - q1.x;
    float a2 = q2.y - p2.y;
    float b2 = p2.x - q2.x;
    float c1 = a1 * p1.x + b1 * p1.y;
    float c2 = a2 * p2.x + b2 * p2.y;

    float det = a1 * b2 - a2 * b1;

    if ( det == 0 ) {
      if ( Line{p1, q1}.is_point_on(p2) ) intersectionPoints.push_back(p2);
      if ( Line{p1, q1}.is_point_on(q2) ) intersectionPoints.push_back(q2);
      if ( Line{p2, q2}.is_point_on(p1) ) intersectionPoints.push_back(p1);
      if ( Line{p2, q2}.is_point_on(q1) ) intersectionPoints.push_back(q1);
    }
    else {
      float pX{};
      float pY{};
      pX = (b2 * c1 - b1 * c2) / det;
      pY = -(a2 * c1 - a1 * c2) / det;
      intersectionPoints.emplace_back(pX, pY);
    }
  }

  [[nodiscard]] VIPRA_PERF_FUNC auto get_collision_area_midpoint(VIPRA::idx index1,
                                                                 VIPRA::idx index2) -> VIPRA::f3d
  {
    Rectangle const& r1 = collisionRectangles[index1];
    Rectangle const& r2 = collisionRectangles[index2];

    std::array<Line, 4> s1{
        Line{r1.p1(), r1.p2()}, {r1.p2(), r1.p3()}, {r1.p3(), r1.p4()}, {r1.p4(), r1.p1()}};
    std::array<Line, 4> s2{
        Line{r2.p1(), r2.p2()}, {r2.p2(), r2.p3()}, {r2.p3(), r2.p4()}, {r2.p4(), r2.p1()}};

    VIPRA::f3dVec intersectionPoints;
    for ( size_t i = 0; i < 4; i++ ) {
      for ( size_t j = 0; j < 4; j++ ) {
        if ( s1[i].does_intersect(s2[j]) ) {
          add_intersection_points(s1[i].start, s1[i].end, s2[j].start, s2[j].end, intersectionPoints);
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

  [[nodiscard]] VIPRA_PERF_FUNC auto check_if_highest_priority(auto const& pedset, auto const& goals,
                                                               VIPRA::idx      pedIdx,
                                                               VIPRA::timestep timestep) -> bool
  {
    auto neighbors = pedset.all_neighbors_within(pedIdx, 1);

    bool flag = true;
    for ( VIPRA::idx otherIdx : neighbors ) {
      if ( otherIdx == pedIdx ) continue;
      if ( goals.is_goal_met(otherIdx) ) continue;

      auto coords1 = pedset.ped_coords(pedIdx);
      auto coords2 = pedset.ped_coords(otherIdx);

      constexpr float DIST2 = (4 * RECTANGLE_RANGE * RECTANGLE_RANGE);
      if ( coords1.distance_to_sqrd(coords2) >= DIST2 ) continue;

      bool cr2in1 = collisionRectangles[pedIdx].is_point_inside(coords2);
      bool cr1in2 = collisionRectangles[otherIdx].is_point_inside(coords1);

      auto goal1 = goals.current_goal(pedIdx);
      auto goal2 = goals.current_goal(otherIdx);

      // If goals don't match, check if coordinates of the pedestrians are in
      // each others collision rectangles
      if ( goal1 != goal2 ) {
        if ( ! cr2in1 ) {
          inRace[pedIdx][otherIdx] = false;
          inRace[otherIdx][pedIdx] = false;
          continue;
        }

        if ( ! cr1in2 ) {
          inRace[otherIdx][pedIdx] = true;
          inRace[pedIdx][otherIdx] = true;
          flag = false;
          continue;
        }
      }

      // Check if pedestrians collide, if not continue onto next passenger
      bool collisionCheck = check_if_collide(pedIdx, otherIdx);
      if ( ! collisionCheck ) {
        if ( inRace[pedIdx][otherIdx] ) {
          inRace[pedIdx][otherIdx] = false;
          inRace[otherIdx][pedIdx] = false;
        }
        continue;
      }

      // If goals match, use distance comparision
      if ( goal1 == goal2 ) {
        if ( coords1.distance_to_sqrd(goal1) > coords2.distance_to_sqrd(goal2) ) {
          inRace[otherIdx][pedIdx] = true;
          inRace[pedIdx][otherIdx] = true;
          flag = false;
        }
        else {
          if ( inRace[pedIdx][otherIdx] ) {
            inRace[pedIdx][otherIdx] = false;
            inRace[otherIdx][pedIdx] = false;
          }
        }
        continue;
      }

      // If goals don't match, use collision midpoint to resolve race condition
      VIPRA::f3d collisionMidpoint;
      if ( ! inRace[pedIdx][otherIdx] ) {
        collisionMidpoint = get_collision_area_midpoint(pedIdx, otherIdx);

        velocityDirections[pedIdx] = pedset.ped_velocity(pedIdx).unit();

        intersectionMidpoints[pedIdx][otherIdx] = collisionMidpoint;
        intersectionMidpoints[otherIdx][pedIdx] = collisionMidpoint;

        inRace[otherIdx][pedIdx] = true;
        inRace[pedIdx][otherIdx] = true;
      }
      else {
        collisionMidpoint = intersectionMidpoints[pedIdx][otherIdx];
      }
      if ( coords1.distance_to_sqrd(collisionMidpoint) > coords2.distance_to_sqrd(collisionMidpoint) )
        flag = false;
      else if ( coords1.distance_to_sqrd(collisionMidpoint) == coords2.distance_to_sqrd(collisionMidpoint) ) {
        if ( pedIdx < otherIdx ) flag = false;
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
  VIPRA_PERF_FUNC auto check_if_collide(VIPRA::idx index1, VIPRA::idx index2) -> bool
  {
    Rectangle const& rect1 = collisionRectangles[index1];
    Rectangle const& rect2 = collisionRectangles[index2];
    return rect1.does_intersect(rect2);
  }

  /**
   * @brief Gets the shoulder points of a pedestrian
   *
   * @param coords : Coordinates of the pedestrian
   * @param velocity : Velocity of the pedestrian
   * @param shoulderWidth : Width of the pedestrian
   * @return Line : Line containing the shoulder points
   */
  [[nodiscard]] VIPRA_PERF_FUNC VIPRA_INLINE static auto get_shoulder_points(
      const VIPRA::f3d& coords, const VIPRA::f3d& velocity, float shoulderWidth) noexcept -> Line
  {
    if ( velocity == VIPRA::f3d{} ) {
      return {(VIPRA::f3d{0, shoulderWidth}), (VIPRA::f3d{0, -shoulderWidth})};
    }

    return {(VIPRA::f3d{-velocity.y, velocity.x}.unit() * shoulderWidth) + coords,
            (VIPRA::f3d{velocity.y, -velocity.x}.unit() * shoulderWidth) + coords};
  }
};
}  // namespace CALM