#pragma once

#include "vipra/geometry/f3d.hpp"

namespace VIPRA::Geometry {
enum class Orientation { COLLINEAR, CLOCKWISE, COUNTERCLOCKWISE };

struct Line {
  VIPRA::f3d start;
  VIPRA::f3d end;

  // TODO(rolland): add more to the line class
  constexpr Line(VIPRA::f3d start, VIPRA::f3d end) : start(start), end(end) {}

  /**
   * @brief Calculates the orientation of a point relative to a line.
   * 
   * @param point1 
   * @param point2 
   * @param point3 
   * @return 
   */
  [[nodiscard]] static constexpr auto orientation_to(Line line, VIPRA::f3d point) noexcept -> Orientation {
    VIPRA::f_pnt val = (line.end.y - line.start.y) * (point.x - line.end.x) -
                       (line.end.x - line.start.x) * (point.y - line.end.y);

    if (val > 0) return Orientation::CLOCKWISE;
    if (val < 0) return Orientation::COUNTERCLOCKWISE;
    return Orientation::COLLINEAR;
  }

  /**
   * @brief Calculates the closest point on the line to another point
   * 
   * @param point 
   * @return VIPRA::f3d 
   */
  [[nodiscard]] inline constexpr auto closest_point(VIPRA::f3d point) const noexcept -> VIPRA::f3d {
    // TODO(rolland): verify
    const VIPRA::f_pnt lineLength = start.distance_to(end);
    const VIPRA::f_pnt t =
        std::max(0.0, std::min(1.0, (point - start).dot(end - start) / (lineLength * lineLength)));
    return start + (end - start) * t;
  }

  /**
   * @brief Checks if a point is on the line.
   * 
   * @param point 
   * @return true 
   * @return false 
   */
  [[nodiscard]] inline constexpr auto is_point_on(VIPRA::f3d point) const -> bool {
    return point.x <= std::max(start.x, end.x) && point.y <= std::max(start.y, end.y) &&
           point.x >= std::min(start.x, end.x) && point.y >= std::min(start.y, end.y);
  }

  /**
   * @brief Checks if the line intersects another
   * 
   * @param other 
   * @return true 
   * @return false 
   */
  [[nodiscard]] constexpr auto does_intersect(Line other) const noexcept -> bool {
    return do_intersect(*this, other);
  }

  /**
   * @brief Calculates the intersection point of two lines
   * @note assumes the lines intersect, check does_intersect before calling
   * 
   * @param other 
   * @return VIPRA::f3d 
   */
  [[nodiscard]] constexpr auto intersection_point(Line other) const noexcept -> VIPRA::f3d {
    assert(does_intersect(other));

    const VIPRA::f_pnt a1 = end.y - start.y;
    const VIPRA::f_pnt b1 = start.x - end.x;
    const VIPRA::f_pnt c1 = a1 * start.x + b1 * start.y;

    const VIPRA::f_pnt a2 = other.end.y - other.start.y;
    const VIPRA::f_pnt b2 = other.start.x - other.end.x;
    const VIPRA::f_pnt c2 = a2 * other.start.x + b2 * other.start.y;

    const VIPRA::f_pnt determinant = a1 * b2 - a2 * b1;

    if (determinant == 0) return VIPRA::f3d{0, 0, 0};

    const VIPRA::f_pnt x = (b2 * c1 - b1 * c2) / determinant;
    const VIPRA::f_pnt y = (a1 * c2 - a2 * c1) / determinant;

    return VIPRA::f3d{x, y, 0};
  }

  /**
   * @brief Checks if two lines intersect
   * 
   * @param line1 
   * @param line2 
   * @return true 
   * @return false 
   */
  [[nodiscard]] static constexpr auto do_intersect(Line line1, Line line2) noexcept -> bool {
    const Orientation ori1 = orientation_to(line1, line2.start);
    const Orientation ori2 = orientation_to(line1, line2.end);
    const Orientation ori3 = orientation_to(line2, line1.start);
    const Orientation ori4 = orientation_to(line2, line1.end);

    if (ori1 != ori2 && ori3 != ori4) return true;

    if (ori1 == Orientation::COLLINEAR && line1.is_point_on(line2.start)) return true;
    if (ori2 == Orientation::COLLINEAR && line1.is_point_on(line2.end)) return true;
    if (ori3 == Orientation::COLLINEAR && line2.is_point_on(line1.start)) return true;
    if (ori4 == Orientation::COLLINEAR && line2.is_point_on(line1.end)) return true;

    return false;
  }

  constexpr Line() = default;
  constexpr ~Line() = default;
  constexpr Line(const Line&) = default;
  constexpr Line(Line&&) = default;
  constexpr auto operator=(const Line&) -> Line& = default;
  constexpr auto operator=(Line&&) -> Line& = default;
};
}  // namespace VIPRA::Geometry