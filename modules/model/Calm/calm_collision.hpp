#pragma once

#include "calm_model_types.hpp"

#include "vipra/macros/performance.hpp"

#include "vipra/geometry/geometry.hpp"

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
    inRace = std::vector<std::vector<bool>>(
        pedset.num_pedestrians(),
        std::vector<bool>(pedset.num_pedestrians(), false));

    intersectionMidpoints = std::vector<VIPRA::f3dVec>(
        pedset.num_pedestrians(), VIPRA::f3dVec(pedset.num_pedestrians()));

    velocityDirections = VIPRA::f3dVec(pedset.num_pedestrians());

    initialize_rectangles(pedset, goals, data);
  }

  void race_detection(auto const& pedset, ModelData const& data,
                      auto const& goals, VIPRA::timestep timestep,
                      auto const& /*map*/)
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

  void initialize_rectangles(auto const& pedset, auto const& goals,
                             ModelData const& data)
  {
    collisionRectangles = std::vector<Rectangle>(pedset.num_pedestrians());
    auto const& shldrs = data.shoulderLens;
    for ( VIPRA::size i = 0; i < collisionRectangles.size(); i++ ) {
      auto coords = pedset.ped_coords(i);
      auto goalCoords = goals.current_goal(i);
      auto goalDirection = goalCoords - coords;

      auto shldr1 =
          (VIPRA::f3d{-goalDirection.y, goalDirection.x}.unit() * shldrs[i]) +
          coords;
      auto shldr2 =
          (VIPRA::f3d{goalDirection.y, -goalDirection.x}.unit() * shldrs[i]) +
          coords;

      const VIPRA::f3d range = (goalDirection.unit() * RECTANGLE_RANGE);
      collisionRectangles[i] =
          Rectangle{shldr1, shldr1 + range, shldr2 + range, shldr2};
    }
  }

  [[nodiscard]] auto status(VIPRA::idx pedIdx) const -> RaceStatus
  {
    return raceStatuses[pedIdx];
  }

  std::vector<RaceStatus>              raceStatuses;
  std::vector<Rectangle>               collisionRectangles;
  std::vector<std::vector<bool>>       inRace;
  std::vector<std::vector<VIPRA::f3d>> intersectionMidpoints;
  std::vector<VIPRA::f3d>              velocityDirections;

  static constexpr VIPRA::size  MAX_COUNT = 500;
  static constexpr VIPRA::f_pnt MINSPEED = 0.00000001F;
  static constexpr VIPRA::f_pnt RECTANGLE_RANGE = 0.4;

  void calc_collision_rectangles(auto const& pedset, auto const& goals,
                                 ModelData const& data)
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

        auto shldr1 =
            (VIPRA::f3d{-goalDirection.y, goalDirection.x}.unit() * shldrs[i]) +
            coords[i];
        auto shldr2 =
            (VIPRA::f3d{goalDirection.y, -goalDirection.x}.unit() * shldrs[i]) +
            coords[i];

        const VIPRA::f3d range = (goalDirection.unit() * RECTANGLE_RANGE);
        collisionRectangles[i] =
            Rectangle{shldr1, shldr1 + range, shldr2 + range, shldr2};
        continue;
      }

      const Line shoulders = get_shoulder_points(coords[i], pedVel, shldrs[i]);
      const VIPRA::f3d range = (pedVel.unit() * RECTANGLE_RANGE);
      collisionRectangles[i] =
          Rectangle{shoulders.start, shoulders.start + range,
                    shoulders.end + range, shoulders.end};
    }
  }

  static void add_intersection_points(VIPRA::f3d start1, VIPRA::f3d end1,
                                      VIPRA::f3d start2, VIPRA::f3d end2,
                                      VIPRA::f3dVec& intersectionPoints)
  {
    // NOLINTBEGIN(readability-identifier-length)
    VIPRA::f_pnt a1 = end1.y - start1.y;
    VIPRA::f_pnt b1 = start1.x - end1.x;
    VIPRA::f_pnt a2 = end2.y - start2.y;
    VIPRA::f_pnt b2 = start2.x - end2.x;
    VIPRA::f_pnt c1 = a1 * start1.x + b1 * start1.y;
    VIPRA::f_pnt c2 = a2 * start2.x + b2 * start2.y;
    // NOLINTEND(readability-identifier-length)

    VIPRA::f_pnt det = a1 * b2 - a2 * b1;

    if ( det == 0 ) {
      if ( Line{start1, end1}.is_point_on(start2) )
        intersectionPoints.push_back(start2);
      if ( Line{start1, end1}.is_point_on(end2) )
        intersectionPoints.push_back(end2);
      if ( Line{start2, end2}.is_point_on(start1) )
        intersectionPoints.push_back(start1);
      if ( Line{start2, end2}.is_point_on(end1) )
        intersectionPoints.push_back(start1);
    }
    else {
      VIPRA::f_pnt pointX{};
      VIPRA::f_pnt pointY{};
      pointX = (b2 * c1 - b1 * c2) / det;
      pointY = -(a2 * c1 - a1 * c2) / det;
      intersectionPoints.emplace_back(pointX, pointY);
    }
  }

  [[nodiscard]] auto get_collision_area_midpoint(
      VIPRA::idx index1, VIPRA::idx index2) -> VIPRA::f3d
  {
    Rectangle const& rec1 = collisionRectangles[index1];
    Rectangle const& rec2 = collisionRectangles[index2];

    auto const& sides1 = rec1.sides();
    auto const& sides2 = rec2.sides();

    VIPRA::f3dVec intersectionPoints;
    for ( size_t i = 0; i < 4; i++ ) {
      for ( size_t j = 0; j < 4; j++ ) {
        if ( sides1[i].does_intersect(sides2[j]) ) {
          add_intersection_points(sides1[i].start, sides1[i].end,
                                  sides2[j].start, sides2[j].end,
                                  intersectionPoints);
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

  [[nodiscard]] auto check_if_highest_priority(
      auto const& pedset, auto const& goals, VIPRA::idx pedIdx,
      VIPRA::timestep /*timestep*/) -> bool
  {
    auto neighbors = pedset.all_neighbors_within(pedIdx, 1);

    bool flag = true;
    for ( VIPRA::idx otherIdx : neighbors ) {
      if ( otherIdx == pedIdx ) continue;
      if ( goals.is_goal_met(otherIdx) ) continue;

      auto coords1 = pedset.ped_coords(pedIdx);
      auto coords2 = pedset.ped_coords(otherIdx);

      constexpr VIPRA::f_pnt DIST2 = (4 * RECTANGLE_RANGE * RECTANGLE_RANGE);
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
        if ( coords1.distance_to_sqrd(goal1) >
             coords2.distance_to_sqrd(goal2) ) {
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
      if ( coords1.distance_to_sqrd(collisionMidpoint) >
           coords2.distance_to_sqrd(collisionMidpoint) )
        flag = false;
      else if ( coords1.distance_to_sqrd(collisionMidpoint) ==
                coords2.distance_to_sqrd(collisionMidpoint) ) {
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
  auto check_if_collide(VIPRA::idx index1, VIPRA::idx index2) -> bool
  {
    Rectangle const& rect1 = collisionRectangles[index1];
    Rectangle const& rect2 = collisionRectangles[index2];
    return VIPRA::Geometry::do_intersect(rect1, rect2);
  }

  /**
   * @brief Gets the shoulder points of a pedestrian
   *
   * @param coords : Coordinates of the pedestrian
   * @param velocity : Velocity of the pedestrian
   * @param shoulderWidth : Width of the pedestrian
   * @return Line : Line containing the shoulder points
   */
  [[nodiscard]] VIPRA_INLINE static auto get_shoulder_points(
      const VIPRA::f3d& coords, const VIPRA::f3d& velocity,
      VIPRA::f_pnt shoulderWidth) noexcept -> Line
  {
    if ( velocity == VIPRA::f3d{} ) {
      return {(VIPRA::f3d{0, shoulderWidth}), (VIPRA::f3d{0, -shoulderWidth})};
    }

    return {
        (VIPRA::f3d{-velocity.y, velocity.x}.unit() * shoulderWidth) + coords,
        (VIPRA::f3d{velocity.y, -velocity.x}.unit() * shoulderWidth) + coords};
  }
};
}  // namespace CALM