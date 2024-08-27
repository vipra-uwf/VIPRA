#pragma once

#include "vipra/geometry/f3d.hpp"
#include "vipra/geometry/line.hpp"

#include "vipra/macros/performance.hpp"

namespace VIPRA::Geometry {
class Circle {
  // TODO(rolland): issue #17 add more
 public:
  constexpr Circle(VIPRA::f3d center, VIPRA::f_pnt radius) : _center(center), _radius(radius) {}

  [[nodiscard]] VIPRA_INLINE constexpr auto center() const -> const VIPRA::f3d& { return _center; }
  [[nodiscard]] VIPRA_INLINE constexpr auto radius() const -> const VIPRA::f_pnt& { return _radius; }

  /**
   * @brief Check if a point is inside the circle
   * 
   * @param point 
   * @return true 
   * @return false 
   */
  [[nodiscard]] VIPRA_INLINE constexpr auto is_inside(VIPRA::f3d point) const -> bool
  {
    return (point.distance_to(_center) <= _radius);
  }

  [[nodiscard]] VIPRA_INLINE constexpr auto does_intersect(VIPRA::Geometry::Line line) const noexcept -> bool
  {
    return line.closest_point(_center).distance_to(_center) <= _radius;
  }

 private:
  VIPRA::f3d   _center{};
  VIPRA::f_pnt _radius{};
};

}  // namespace VIPRA::Geometry