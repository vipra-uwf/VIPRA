/*
 * Agent.cpp
 * RVO2 Library
 *
 * Copyright 2008 University of North Carolina at Chapel Hill
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Please send all bug reports to <geom@cs.unc.edu>.
 *
 * The authors may be contacted via:
 *
 * Jur van den Berg, Stephen J. Guy, Jamie Snape, Ming C. Lin, Dinesh Manocha
 * Dept. of Computer Science
 * 201 S. Columbia St.
 * Frederick P. Brooks, Jr. Computer Science Bldg.
 * Chapel Hill, N.C. 27599-3175
 * United States of America
 *
 * <http://gamma.cs.unc.edu/RVO2/>
 */

/*
  Modified by Rolland Goodenough to integrate with the VIPRA simulation framework
*/

#include "orca.hpp"

#include <cstddef>
#include <stdexcept>

#include "vipra/geometry/f3d.hpp"
#include "vipra/macros/module.hpp"
#include "vipra/modules/collision_avoidance.hpp"
#include "vipra/modules/map.hpp"
#include "vipra/modules/pedestrians.hpp"
#include "vipra/types/float.hpp"
#include "vipra/types/idx.hpp"
#include "vipra/types/time.hpp"

VIPRA_REGISTER_MODULE(ORCA, CollisionAvoidance)

namespace {
constexpr VIPRA::f_pnt RVO_EPSILON = 0.00001;

inline auto det(const VIPRA::f3d& vector1,
                const VIPRA::f3d& vector2) -> VIPRA::f_pnt
{
  return vector1.x * vector2.y - vector1.y * vector2.x;
}

auto linear_program1(const std::vector<VIPRA::Geometry::Line>& lines,
                     size_t lineNo, VIPRA::f_pnt _radius,
                     const VIPRA::f3d& optVelocity, bool directionOpt,
                     VIPRA::f3d& result) -> bool
{
  const VIPRA::f_pnt dotProduct =
      lines[lineNo].start.dot(lines[lineNo].direction());
  const VIPRA::f_pnt discriminant = (dotProduct * dotProduct) +
                                    (_radius * _radius) -
                                    (lines[lineNo].start).mag_sqrd();

  if ( discriminant < 0.0F ) {
    /* Max speed circle fully invalidates line lineNo. */
    return false;
  }

  const VIPRA::f_pnt sqrtDiscriminant = std::sqrt(discriminant);
  VIPRA::f_pnt       tLeft = -dotProduct - sqrtDiscriminant;
  VIPRA::f_pnt       tRight = -dotProduct + sqrtDiscriminant;

  for ( size_t i = 0; i < lineNo; ++i ) {
    const VIPRA::f_pnt denominator =
        det(lines[lineNo].direction(), lines[i].direction());
    const VIPRA::f_pnt numerator =
        det(lines[i].direction(), lines[lineNo].start - lines[i].start);

    if ( std::fabs(denominator) <= RVO_EPSILON ) {
      /* Lines lineNo and i are (almost) parallel. */
      if ( numerator < 0.0F ) {
        return false;
      }

      continue;
    }

    const VIPRA::f_pnt t = numerator / denominator;

    if ( denominator >= 0.0F ) {
      /* Line i bounds line lineNo on the right. */
      tRight = std::min(tRight, t);
    }
    else {
      /* Line i bounds line lineNo on the left. */
      tLeft = std::max(tLeft, t);
    }

    if ( tLeft > tRight ) {
      return false;
    }
  }

  if ( directionOpt ) {
    /* Optimize direction. */
    if ( optVelocity.dot(lines[lineNo].direction()) > 0.0F ) {
      /* Take right extreme. */
      result = lines[lineNo].start + tRight * lines[lineNo].direction();
    }
    else {
      /* Take left extreme. */
      result = lines[lineNo].start + tLeft * lines[lineNo].direction();
    }
  }
  else {
    /* Optimize closest point. */
    const VIPRA::f_pnt t =
        lines[lineNo].direction().dot((optVelocity - lines[lineNo].start));

    if ( t < tLeft ) {
      result = lines[lineNo].start + tLeft * lines[lineNo].direction();
    }
    else if ( t > tRight ) {
      result = lines[lineNo].start + tRight * lines[lineNo].direction();
    }
    else {
      result = lines[lineNo].start + t * lines[lineNo].direction();
    }
  }

  return true;
}

auto linear_program2(const std::vector<VIPRA::Geometry::Line>& lines,
                     VIPRA::f_pnt _radius, const VIPRA::f3d& optVelocity,
                     bool directionOpt, VIPRA::f3d& result) -> size_t
{
  if ( directionOpt ) {
    /*
			 * Optimize direction. Note that the optimization velocity is of unit
			 * length in this case.
			 */
    result = optVelocity * _radius;
  }
  else if ( optVelocity.mag_sqrd() > (_radius) ) {
    /* Optimize closest point and outside circle. */
    result = optVelocity.unit() * _radius;
  }
  else {
    /* Optimize closest point and inside circle. */
    result = optVelocity;
  }

  for ( size_t i = 0; i < lines.size(); ++i ) {
    if ( det(lines[i].direction(), lines[i].start - result) > 0.0F ) {
      /* Result does not satisfy constraint i. Compute new optimal result. */
      const VIPRA::f3d tempResult = result;

      if ( ! linear_program1(lines, i, _radius, optVelocity, directionOpt,
                             result) ) {
        result = tempResult;
        return i;
      }
    }
  }

  return lines.size();
}

void linear_program3(const std::vector<VIPRA::Geometry::Line>& lines,
                     size_t numObstLines, size_t beginLine,
                     VIPRA::f_pnt _radius, VIPRA::f3d& result)
{
  VIPRA::f_pnt distance = 0.0F;

  for ( size_t i = beginLine; i < lines.size(); ++i ) {
    if ( det(lines[i].direction(), lines[i].start - result) > distance ) {
      /* Result does not satisfy constraint of line i. */
      std::vector<VIPRA::Geometry::Line> projLines(
          lines.begin(), lines.begin() + static_cast<ptrdiff_t>(numObstLines));

      for ( size_t j = numObstLines; j < i; ++j ) {
        VIPRA::Geometry::Line line;

        VIPRA::f_pnt determinant =
            det(lines[i].direction(), lines[j].direction());

        if ( std::fabs(determinant) <= RVO_EPSILON ) {
          /* Line i and line j are parallel. */
          if ( lines[i].direction().dot(lines[j].direction()) > 0.0F ) {
            /* Line i and line j point in the same direction. */
            continue;
          }

          /* Line i and line j point in opposite direction. */
          line.start = 0.5F * (lines[i].start + lines[j].start);
        }
        else {
          line.start = lines[i].start + (det(lines[j].direction(),
                                             lines[i].start - lines[j].start) /
                                         determinant) *
                                            lines[i].direction();
        }

        line.direction() = (lines[j].direction() - lines[i].direction()).unit();
        projLines.push_back(line);
      }

      const VIPRA::f3d tempResult = result;

      if ( linear_program2(
               projLines, _radius,
               VIPRA::f3d(-lines[i].direction().y, lines[i].direction().x),
               true, result) < projLines.size() ) {
        /* This should in principle not happen.  The result is by definition
					 * already in the feasible region of this linear program. If it fails,
					 * it is due to small VIPRA::f_pnting point error, and the current result is
					 * kept.
					 */
        result = tempResult;
      }

      distance = det(lines[i].direction(), lines[i].start - result);
    }
  }
}
}  // namespace

void ORCA::obstacle_orca_lines(
    VIPRA::idx pedIdx, VIPRA::Modules::Pedestrians const& pedset,
    VIPRA::Modules::Map const& map, VIPRA::delta_t timeStep,
    std::vector<VIPRA::Geometry::Line>& orcaLines) const
{
  // TODO(rolland): this

  // const VIPRA::f3d& position = pedset.ped_coords(pedIdx);

  // /* Create obstacle ORCA lines. */
  // for ( size_t i = 0; i < obstacleNeighbors_.size(); ++i ) {
  //   const VIPRA::Geometry::Polygon& obstacle1 = obstacleNeighbors_[i].second;
  //   const VIPRA::Geometry::Polygon& obstacle2 = obstacle1->nextObstacle_;

  //   const VIPRA::f3d relativePosition1 = obstacle1.points()[0] - position;
  //   const VIPRA::f3d relativePosition2 = obstacle2.points()[0] - position;

  //   /*
  // 		 * Check if velocity obstacle of obstacle is already taken care of by
  // 		 * previously constructed obstacle ORCA lines.
  // 		 */
  //   bool alreadyCovered = false;

  //   for ( size_t j = 0; j < orcaLines.size(); ++j ) {
  //     if ( det(_invTimeHorizon * relativePosition1 - orcaLines[j].start,
  //              orcaLines[j].direction()) -
  //                  _invTimeHorizon * _radius >=
  //              -RVO_EPSILON &&
  //          det(_invTimeHorizon * relativePosition2 - orcaLines[j].start,
  //              orcaLines[j].direction()) -
  //                  _invTimeHorizon * _radius >=
  //              -RVO_EPSILON ) {
  //       alreadyCovered = true;
  //       break;
  //     }
  //   }

  //   if ( alreadyCovered ) {
  //     continue;
  //   }

  //   /* Not yet covered. Check for collisions. */

  //   const VIPRA::f_pnt distSq1 = relativePosition1.mag_sqrd();
  //   const VIPRA::f_pnt distSq2 = relativePosition2.mag_sqrd();

  //   const VIPRA::f_pnt radiusSq = sqrt(_radius);

  //   const VIPRA::f3d obstacleVector =
  //       obstacle2.points()[0] - obstacle1.points()[0];
  //   const VIPRA::f_pnt s =
  //       (-relativePosition1.dot(obstacleVector)) / obstacleVector.mag_sqrd();
  //   const VIPRA::f_pnt distSqLine =
  //       (-relativePosition1 - s * obstacleVector).mag_sqrd();

  //   VIPRA::Geometry::Line line;

  //   if ( s < 0.0f && distSq1 <= radiusSq ) {
  //     /* Collision with left vertex. Ignore if non-convex. */
  //     if ( obstacle1->isConvex_ ) {
  //       line.start = VIPRA::f3d(0.0f, 0.0f);
  //       line.direction() =
  //           VIPRA::f3d(-relativePosition1.y, relativePosition1.x).unit();
  //       orcaLines.push_back(line);
  //     }

  //     continue;
  //   }
  //   else if ( s > 1.0f && distSq2 <= radiusSq ) {
  //     /* Collision with right vertex. Ignore if non-convex
  // 			 * or if it will be taken care of by neighoring obstace */
  //     if ( obstacle2->isConvex_ &&
  //          det(relativePosition2, obstacle2->unitDir_) >= 0.0f ) {
  //       line.start = VIPRA::f3d(0.0f, 0.0f);
  //       line.direction() =
  //           VIPRA::f3d(-relativePosition2.y, relativePosition2.x).unit();
  //       orcaLines.push_back(line);
  //     }

  //     continue;
  //   }
  //   else if ( s >= 0.0f && s < 1.0f && distSqLine <= radiusSq ) {
  //     /* Collision with obstacle segment. */
  //     line.start = VIPRA::f3d(0.0f, 0.0f);
  //     line.direction() = -obstacle1->unitDir_;
  //     orcaLines.push_back(line);
  //     continue;
  //   }

  //   /*
  // 		 * No collision.
  // 		 * Compute legs. When obliquely viewed, both legs can come from a single
  // 		 * vertex. Legs extend cut-off line when nonconvex vertex.
  // 		 */

  //   VIPRA::f3d leftLegDirection, rightLegDirection;

  //   if ( s < 0.0f && distSqLine <= radiusSq ) {
  //     /*
  // 			 * Obstacle viewed obliquely so that left vertex
  // 			 * defines velocity obstacle.
  // 			 */
  //     if ( ! obstacle1->isConvex_ ) {
  //       /* Ignore obstacle. */
  //       continue;
  //     }

  //     obstacle2 = obstacle1;

  //     const VIPRA::f_pnt leg1 = std::sqrt(distSq1 - radiusSq);
  //     leftLegDirection =
  //         VIPRA::f3d(
  //             relativePosition1.x * leg1 - relativePosition1.y * _radius,
  //             relativePosition1.x * _radius + relativePosition1.y * leg1) /
  //         distSq1;
  //     rightLegDirection =
  //         VIPRA::f3d(
  //             relativePosition1.x * leg1 + relativePosition1.y * _radius,
  //             -relativePosition1.x * _radius + relativePosition1.y * leg1) /
  //         distSq1;
  //   }
  //   else if ( s > 1.0f && distSqLine <= radiusSq ) {
  //     /*
  // 			 * Obstacle viewed obliquely so that
  // 			 * right vertex defines velocity obstacle.
  // 			 */
  //     if ( ! obstacle2->isConvex_ ) {
  //       /* Ignore obstacle. */
  //       continue;
  //     }

  //     obstacle1 = obstacle2;

  //     const VIPRA::f_pnt leg2 = std::sqrt(distSq2 - radiusSq);
  //     leftLegDirection =
  //         VIPRA::f3d(
  //             relativePosition2.x * leg2 - relativePosition2.y * _radius,
  //             relativePosition2.x * _radius + relativePosition2.y * leg2) /
  //         distSq2;
  //     rightLegDirection =
  //         VIPRA::f3d(
  //             relativePosition2.x * leg2 + relativePosition2.y * _radius,
  //             -relativePosition2.x * _radius + relativePosition2.y * leg2) /
  //         distSq2;
  //   }
  //   else {
  //     /* Usual situation. */
  //     if ( obstacle1->isConvex_ ) {
  //       const VIPRA::f_pnt leg1 = std::sqrt(distSq1 - radiusSq);
  //       leftLegDirection =
  //           VIPRA::f3d(
  //               relativePosition1.x * leg1 - relativePosition1.y * _radius,
  //               relativePosition1.x * _radius + relativePosition1.y * leg1) /
  //           distSq1;
  //     }
  //     else {
  //       /* Left vertex non-convex; left leg extends cut-off line. */
  //       leftLegDirection = -obstacle1->unitDir_;
  //     }

  //     if ( obstacle2->isConvex_ ) {
  //       const VIPRA::f_pnt leg2 = std::sqrt(distSq2 - radiusSq);
  //       rightLegDirection =
  //           VIPRA::f3d(
  //               relativePosition2.x * leg2 + relativePosition2.y * _radius,
  //               -relativePosition2.x * _radius + relativePosition2.y * leg2) /
  //           distSq2;
  //     }
  //     else {
  //       /* Right vertex non-convex; right leg extends cut-off line. */
  //       rightLegDirection = obstacle1->unitDir_;
  //     }
  //   }

  //   /*
  // 		 * Legs can never point into neighboring edge when convex vertex,
  // 		 * take cutoff-line of neighboring edge instead. If velocity projected on
  // 		 * "foreign" leg, no constraint is added.
  // 		 */

  //   const VIPRA::Geometry::Polygon& const leftNeighbor =
  //       obstacle1->prevObstacle_;

  //   bool isLeftLegForeign = false;
  //   bool isRightLegForeign = false;

  //   if ( obstacle1->isConvex_ &&
  //        det(leftLegDirection, -leftNeighbor->unitDir_) >= 0.0f ) {
  //     /* Left leg points into obstacle. */
  //     leftLegDirection = -leftNeighbor->unitDir_;
  //     isLeftLegForeign = true;
  //   }

  //   if ( obstacle2->isConvex_ &&
  //        det(rightLegDirection, obstacle2->unitDir_) <= 0.0f ) {
  //     /* Right leg points into obstacle. */
  //     rightLegDirection = obstacle2->unitDir_;
  //     isRightLegForeign = true;
  //   }

  //   /* Compute cut-off centers. */
  //   const VIPRA::f3d leftCutoff =
  //       _invTimeHorizon * (obstacle1.points()[0] - position);
  //   const VIPRA::f3d rightCutoff =
  //       _invTimeHorizon * (obstacle2.points()[0] - position);
  //   const VIPRA::f3d cutoffVec = rightCutoff - leftCutoff;

  //   /* Project current velocity on velocity obstacle. */

  //   /* Check if current velocity is projected on cutoff circles. */
  //   const VIPRA::f_pnt t =
  //       (obstacle1 == obstacle2
  //            ? 0.5f
  //            : ((velocity_ - leftCutoff) * cutoffVec) / absSq(cutoffVec));
  //   const VIPRA::f_pnt tLeft = ((velocity_ - leftCutoff) * leftLegDirection);
  //   const VIPRA::f_pnt tRight = ((velocity_ - rightCutoff) * rightLegDirection);

  //   if ( (t < 0.0f && tLeft < 0.0f) ||
  //        (obstacle1 == obstacle2 && tLeft < 0.0f && tRight < 0.0f) ) {
  //     /* Project on left cut-off circle. */
  //     const VIPRA::f3d unitW = normalize(velocity_ - leftCutoff);

  //     line.direction() = VIPRA::f3d(unitW.y, -unitW.x);
  //     line.start = leftCutoff + _radius * _invTimeHorizon * unitW;
  //     orcaLines.push_back(line);
  //     continue;
  //   }
  //   else if ( t > 1.0f && tRight < 0.0f ) {
  //     /* Project on right cut-off circle. */
  //     const VIPRA::f3d unitW = normalize(velocity_ - rightCutoff);

  //     line.direction() = VIPRA::f3d(unitW.y, -unitW.x);
  //     line.start = rightCutoff + _radius * _invTimeHorizon * unitW;
  //     orcaLines.push_back(line);
  //     continue;
  //   }

  //   /*
  // 		 * Project on left leg, right leg, or cut-off line, whichever is closest
  // 		 * to velocity.
  // 		 */
  //   const VIPRA::f_pnt distSqCutoff =
  //       ((t < 0.0f || t > 1.0f || obstacle1 == obstacle2)
  //            ? std::numeric_limits<VIPRA::f_pnt>::infinity()
  //            : absSq(velocity_ - (leftCutoff + t * cutoffVec)));
  //   const VIPRA::f_pnt distSqLeft =
  //       ((tLeft < 0.0f)
  //            ? std::numeric_limits<VIPRA::f_pnt>::infinity()
  //            : absSq(velocity_ - (leftCutoff + tLeft * leftLegDirection)));
  //   const VIPRA::f_pnt distSqRight =
  //       ((tRight < 0.0f)
  //            ? std::numeric_limits<VIPRA::f_pnt>::infinity()
  //            : absSq(velocity_ - (rightCutoff + tRight * rightLegDirection)));

  //   if ( distSqCutoff <= distSqLeft && distSqCutoff <= distSqRight ) {
  //     /* Project on cut-off line. */
  //     line.direction() = -obstacle1->unitDir_;
  //     line.start =
  //         leftCutoff + _radius * _invTimeHorizon *
  //                          VIPRA::f3d(-line.direction().y, line.direction().x);
  //     orcaLines.push_back(line);
  //     continue;
  //   }
  //   else if ( distSqLeft <= distSqRight ) {
  //     /* Project on left leg. */
  //     if ( isLeftLegForeign ) {
  //       continue;
  //     }

  //     line.direction() = leftLegDirection;
  //     line.start =
  //         leftCutoff + _radius * _invTimeHorizon *
  //                          VIPRA::f3d(-line.direction().y, line.direction().x);
  //     orcaLines.push_back(line);
  //     continue;
  //   }
  //   else {
  //     /* Project on right leg. */
  //     if ( isRightLegForeign ) {
  //       continue;
  //     }

  //     line.direction() = -rightLegDirection;
  //     line.start =
  //         rightCutoff + _radius * _invTimeHorizon *
  //                           VIPRA::f3d(-line.direction().y, line.direction().x);
  //     orcaLines.push_back(line);
  //     continue;
  //   }
  // }
}

void ORCA::pedestrian_orca_lines(
    VIPRA::idx pedIdx, VIPRA::Modules::Pedestrians const& pedset,
    VIPRA::Modules::Map const& /*map*/, VIPRA::delta_t    timeStep,
    std::vector<VIPRA::Geometry::Line>& orcaLines) const
{
  const auto        neighbors = pedset.all_neighbors_within(pedIdx, 3.0);
  const VIPRA::f3d& position = pedset.ped_coords(pedIdx);
  const VIPRA::f3d& velocity = pedset.ped_velocity(pedIdx);

  /* Create agent ORCA lines. */
  for ( VIPRA::idx neighborIdx : neighbors ) {
    if ( neighborIdx == pedIdx ) continue;

    const VIPRA::f3d otherPos = pedset.ped_coords(neighborIdx);
    const VIPRA::f3d otherVel = pedset.ped_velocity(neighborIdx);

    const VIPRA::f3d   relativePosition = otherPos - position;
    const VIPRA::f3d   relativeVelocity = velocity - otherVel;
    const VIPRA::f_pnt distSq = relativePosition.mag_sqrd();
    const VIPRA::f_pnt combinedRadius = _radius + _radius;
    const VIPRA::f_pnt combinedRadiusSq = combinedRadius * combinedRadius;

    VIPRA::Geometry::Line line;
    VIPRA::f3d            u;

    if ( distSq > combinedRadiusSq ) {
      /* No collision. */
      const VIPRA::f3d w =
          relativeVelocity - (_invTimeHorizon * relativePosition);
      /* Vector from cutoff center to relative velocity. */
      const VIPRA::f_pnt wLengthSq = w.mag_sqrd();

      const VIPRA::f_pnt dotProduct1 = w.dot(relativePosition);

      if ( dotProduct1 < 0.0F &&
           (dotProduct1 * dotProduct1) > combinedRadiusSq * wLengthSq ) {
        /* Project on cut-off circle. */
        const VIPRA::f_pnt wLength = std::sqrt(wLengthSq);
        const VIPRA::f3d   unitW = w.unit();

        line.direction() = VIPRA::f3d(unitW.y, -unitW.x);
        u = (combinedRadius * _invTimeHorizon - wLength) * unitW;
      }
      else {
        /* Project on legs. */
        const VIPRA::f_pnt leg = std::sqrt(distSq - combinedRadiusSq);

        if ( det(relativePosition, w) > 0.0F ) {
          /* Project on left leg. */
          line.direction() = VIPRA::f3d(relativePosition.x * leg -
                                            relativePosition.y * combinedRadius,
                                        relativePosition.x * combinedRadius +
                                            relativePosition.y * leg) /
                             distSq;
        }
        else {
          /* Project on right leg. */
          line.direction() =
              -VIPRA::f3d(relativePosition.x * leg +
                              relativePosition.y * combinedRadius,
                          -relativePosition.x * combinedRadius +
                              relativePosition.y * leg) /
              distSq;
        }

        const VIPRA::f_pnt dotProduct2 = relativeVelocity.dot(line.direction());

        u = dotProduct2 * line.direction() - relativeVelocity;
      }
    }
    else {
      /* Collision. Project on cut-off circle of time timeStep. */
      const VIPRA::f_pnt invTimeStep = 1.0F / timeStep;

      /* Vector from cutoff center to relative velocity. */
      const VIPRA::f3d w = relativeVelocity - invTimeStep * relativePosition;

      const VIPRA::f_pnt wLength = w.mag();
      const VIPRA::f3d   unitW = w.unit();

      line.direction() = VIPRA::f3d(unitW.y, -unitW.x);
      u = (combinedRadius * invTimeStep - wLength) * unitW;
    }

    line.start = velocity + 0.5F * u;
    orcaLines.push_back(line);
  }
}

auto ORCA::compute_new_velocity(VIPRA::idx pedIdx, VIPRA::State& state,
                                VIPRA::Modules::Pedestrians const& pedset,
                                VIPRA::delta_t                     timestep,
                                VIPRA::Modules::Map const& map) -> VIPRA::f3d
{
  std::vector<VIPRA::Geometry::Line> orcaLines;
  obstacle_orca_lines(pedIdx, pedset, map, timestep, orcaLines);
  pedestrian_orca_lines(pedIdx, pedset, map, timestep, orcaLines);

  const size_t numObstLines = orcaLines.size();

  VIPRA::f3d newVelocity;

  size_t lineFail = linear_program2(
      orcaLines, _maxSpeed, state.velocities[pedIdx], false, newVelocity);

  if ( lineFail < orcaLines.size() ) {
    linear_program3(orcaLines, numObstLines, lineFail, _maxSpeed, newVelocity);
  }

  return newVelocity;
}
