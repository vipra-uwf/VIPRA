#pragma once

#include <array>

#include "vipra/geometry/definitions.hpp"
#include "vipra/geometry/f3d.hpp"
#include "vipra/geometry/line.hpp"
#include "vipra/geometry/rectangle.hpp"
#include "vipra/types/float.hpp"

namespace VIPRA::Geometry {

class Circle;
class Rectangle;
class Polygon;

class Triangle {
 public:
  VIPRA_POLY_FUNC auto is_point_inside(f3d point) const noexcept -> bool;
  VIPRA_POLY_FUNC auto bounding_box() const noexcept -> Rectangle;
  VIPRA_POLY_FUNC auto sides() const noexcept -> std::array<Line, 3>;
  VIPRA_POLY_FUNC auto center() const noexcept -> f3d;

  [[nodiscard]] auto random_point(Random::Engine& engine) const noexcept -> f3d;

  VIPRA_POLY_FUNC auto area() const noexcept -> f_pnt { return _area; }
  VIPRA_POLY_FUNC auto points() const noexcept -> std::array<f3d, 3> const&
  {
    return _points;
  }

 private:
  std::array<f3d, 3> _points;
  f_pnt              _area{};

 public:
};

VIPRA_POLY_FUNC auto Triangle::is_point_inside(f3d point) const noexcept -> bool
{
  auto const sign = [](f3d point1, f3d point2, f3d point3) -> f_pnt {
    return (point1.x - point3.x) * (point2.y - point3.y) -
           (point2.x - point3.x) * (point1.y - point3.y);
  };

  const f_pnt dir1 = sign(point, _points[0], _points[1]);
  const f_pnt dir2 = sign(point, _points[1], _points[2]);
  const f_pnt dir3 = sign(point, _points[2], _points[0]);

  return ((dir1 >= 0) && (dir2 >= 0) && (dir3 >= 0)) ||
         ((dir1 <= 0) && (dir2 <= 0) && (dir3 <= 0));
}

inline auto Triangle::random_point(Random::Engine& engine) const noexcept -> f3d
{
  const Rectangle box = bounding_box();
  f3d             point = box.random_point(engine);

  while ( ! is_point_inside(point) ) {
    point = box.random_point(engine);
  }

  return point;
}

VIPRA_POLY_FUNC auto Triangle::bounding_box() const noexcept -> Rectangle
{
  f3d botLeft{std::numeric_limits<VIPRA::f_pnt>::max(),
              std::numeric_limits<VIPRA::f_pnt>::max()};
  f3d topRight{std::numeric_limits<VIPRA::f_pnt>::min(),
               std::numeric_limits<VIPRA::f_pnt>::min()};
  for ( auto const& point : _points ) {
    topRight.x = std::max(topRight.x, point.x);
    topRight.y = std::max(topRight.y, point.y);
    botLeft.x = std::min(botLeft.x, point.x);
    botLeft.y = std::min(botLeft.y, point.y);
  }

  f3d topLeft = f3d{botLeft.x, topRight.y};
  f3d botRight = f3d{topRight.x, botLeft.y};

  return Rectangle{botLeft, topLeft, topRight, botRight};
}

VIPRA_POLY_FUNC auto Triangle::sides() const noexcept -> std::array<Line, 3>
{
  std::array<Line, 3> lines;
  for ( size_t i = 0; i < 3; ++i ) {
    lines[i] = Line{_points[i], _points[i + 1]};
  }
  lines.back() = Line{_points.back(), _points.front()};
  return lines;
}

VIPRA_POLY_FUNC auto Triangle::center() const noexcept -> f3d
{
  f3d center;
  for ( f3d point : _points ) {
    center += point;
  }
  return center / 3;
}

}  // namespace VIPRA::Geometry