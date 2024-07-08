#pragma once

#include "vipra/geometry/f3d.hpp"
#include "vipra/geometry/line.hpp"

namespace VIPRA::Geometry {
class Circle {
  // TODO(rolland): add more
 public:
  constexpr Circle(VIPRA::f3d center, VIPRA::f_pnt radius) : _center(center), _radius(radius) {}

  [[nodiscard]] inline constexpr auto center() const -> const VIPRA::f3d& { return _center; }
  [[nodiscard]] inline constexpr auto radius() const -> const VIPRA::f_pnt& { return _radius; }

  /**
   * @brief Check if a point is inside the circle
   * 
   * @param point 
   * @return true 
   * @return false 
   */
  [[nodiscard]] inline constexpr auto is_inside(VIPRA::f3d point) const -> bool
  {
    return (point.distance_to(_center) <= _radius);
  }

  [[nodiscard]] inline constexpr auto does_intersect(VIPRA::Geometry::Line line) const noexcept -> bool
  {
    return line.closest_point(_center).distance_to(_center) <= _radius;
  }

 private:
  VIPRA::f3d   _center{};
  VIPRA::f_pnt _radius{};
};

}  // namespace VIPRA::Geometry