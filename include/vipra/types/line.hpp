#pragma once

#include "vipra/types/f3d.hpp"

namespace VIPRA {
enum class Orientation { COLLINEAR, CLOCKWISE, COUNTERCLOCKWISE };

struct Line {
  VIPRA::f3d start;
  VIPRA::f3d end;

  // TODO(rolland): add more to the line class

  /**
   * @brief Calculates the orientation of a point relative to a line.
   * 
   * @param point1 
   * @param point2 
   * @param point3 
   * @return 
   */
  [[nodiscard]] static constexpr auto orientation_to(VIPRA::Line line, VIPRA::f3d point) noexcept
      -> Orientation {
    VIPRA::f_pnt val = (line.end.y - line.start.y) * (point.x - line.end.x) -
                       (line.end.x - line.start.x) * (point.y - line.end.y);

    if (val > 0) return Orientation::CLOCKWISE;
    if (val < 0) return Orientation::COUNTERCLOCKWISE;
    return Orientation::COLLINEAR;
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
  [[nodiscard]] auto does_intersect(Line other) const noexcept -> bool { return do_intersect(*this, other); }

  /**
   * @brief Checks if two lines intersect
   * 
   * @param line1 
   * @param line2 
   * @return true 
   * @return false 
   */
  [[nodiscard]] static auto do_intersect(Line line1, Line line2) noexcept -> bool {
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
};
}  // namespace VIPRA