#pragma once

#include <array>
#include <numbers>

#include "vipra/geometry/definitions.hpp"
#include "vipra/geometry/f3d.hpp"
#include "vipra/geometry/line.hpp"
#include "vipra/random/random.hpp"
#include "vipra/types/float.hpp"

namespace VIPRA::Geometry {

class Polygon;
class Circle;
class Triangle;

class Rectangle {
 public:
  VIPRA_POLY_FUNC auto is_point_inside(f3d point) const noexcept -> bool;
  VIPRA_POLY_FUNC auto random_point(Random::Engine& engine) const noexcept
      -> f3d;
  VIPRA_POLY_FUNC auto sides() const noexcept -> std::array<Line, 4>;
  VIPRA_POLY_FUNC auto rotation() const noexcept -> f_pnt;

  VIPRA_POLY_FUNC auto center() const noexcept -> f3d const& { return _center; }
  VIPRA_POLY_FUNC auto area() const noexcept -> f_pnt { return _area; }
  VIPRA_POLY_FUNC auto width() const noexcept -> f_pnt { return _width; }
  VIPRA_POLY_FUNC auto height() const noexcept -> f_pnt { return _height; }
  VIPRA_POLY_FUNC auto points() const noexcept -> std::array<f3d, 4> const&
  {
    return _points;
  }

 private:
  std::array<f3d, 4> _points;
  f3d                _center;
  f_pnt              _area{};
  f_pnt              _width{};
  f_pnt              _height{};

  constexpr void calc_dims()
  {
    _width = _points[0].distance_to(_points[3]);
    _height = _points[0].distance_to(_points[1]);
  }

 public:
  constexpr explicit Rectangle(f3d point1, f3d point2, f3d point3, f3d point4)
      : _points({point1, point2, point3, point4})
  {
    calc_dims();
    _area = _height * _width;
  }
  constexpr explicit Rectangle(std::array<f3d, 4> const& points)
      : _points(points)
  {
    calc_dims();
    _area = _height * _width;
  }
  constexpr explicit Rectangle(std::array<f3d, 4>&& points) : _points(points)
  {
    calc_dims();
    _area = _height * _width;
  }
  constexpr explicit Rectangle(std::vector<Line> const& lines)
  {
    std::transform(lines.begin(), lines.end(), _points.begin(),
                   [](Line const& line) { return line.start; });
    calc_dims();
    _area = _height * _width;
  }

  constexpr explicit Rectangle(std::vector<Line>&& lines)
  {
    std::transform(lines.begin(), lines.end(), _points.begin(),
                   [](Line const& line) { return line.start; });
    calc_dims();
    _area = _height * _width;
  }

  constexpr explicit Rectangle(std::array<Line, 4> const& lines)
  {
    std::transform(lines.begin(), lines.end(), _points.begin(),
                   [](Line const& line) { return line.start; });
    calc_dims();
    _area = _height * _width;
  }

  constexpr explicit Rectangle(std::array<Line, 4>&& lines)
  {
    std::transform(lines.begin(), lines.end(), _points.begin(),
                   [](Line const& line) { return line.start; });
    calc_dims();
    _area = _height * _width;
  }
  constexpr Rectangle(VIPRA::f3d const& center, VIPRA::f3d const& dimensions,
                      VIPRA::f_pnt rotation);

  ~Rectangle() = default;
  constexpr Rectangle() = default;
  constexpr Rectangle(Rectangle const&) = default;
  constexpr auto operator=(Rectangle const&) -> Rectangle& = default;
  constexpr Rectangle(Rectangle&&) noexcept = default;
  constexpr auto operator=(Rectangle&&) noexcept -> Rectangle& = default;
};

VIPRA_POLY_FUNC auto Rectangle::is_point_inside(f3d point) const noexcept
    -> bool
{
  // NOLINTBEGIN(readability-identifier-length)
  const f3d ab = _points[1] - _points[0];
  const f3d am = point - _points[0];
  const f3d bc = _points[2] - _points[1];
  const f3d bm = point - _points[1];

  return 0 <= ab.dot(am) && ab.dot(am) <= ab.dot(ab) && 0 <= bc.dot(bm) &&
         bc.dot(bm) <= bc.dot(bc);
  // NOLINTEND(readability-identifier-length)
}

VIPRA_POLY_FUNC auto Rectangle::random_point(
    Random::Engine& engine) const noexcept -> f3d
{
  // TODO(rolland): implement a better method for this

  auto const                                 sideList = sides();
  Random::uniform_distribution<VIPRA::f_pnt> xDist{sideList[3].end.x,
                                                   sideList[3].start.x};
  Random::uniform_distribution<VIPRA::f_pnt> yDist{sideList[2].end.y,
                                                   sideList[2].start.y};

  f3d point;

  do {
    point.x = xDist(engine);
    point.y = yDist(engine);
  } while ( ! is_point_inside(point) );
  return point;
}

VIPRA_POLY_FUNC auto Rectangle::sides() const noexcept -> std::array<Line, 4>
{
  std::array<Line, 4> lines;
  for ( size_t i = 0; i < 4; ++i ) {
    lines[i] = Line{_points[i], _points[i + 1]};
  }
  lines.back() = Line{_points.back(), _points.front()};
  return lines;
}

VIPRA_POLY_FUNC auto Rectangle::rotation() const noexcept -> f_pnt
{
  constexpr f_pnt HALF = 180.0;
  constexpr f_pnt FULL = 360.0;

  const f_pnt xDif = _points[0].x - _points[2].x;
  const f_pnt yDif = _points[0].y - _points[2].y;

  f_pnt rotation = std::atan(xDif / yDif) * HALF / std::numbers::pi;
  if ( yDif < 0 ) {
    rotation += HALF;
  }
  else if ( xDif < 0 ) {
    rotation += FULL;
  }

  return rotation;
}

constexpr Rectangle::Rectangle(VIPRA::f3d const& center,
                               VIPRA::f3d const& dimensions,
                               VIPRA::f_pnt      rotation)
    : _center(center)
{
  const VIPRA::f3d point1 =
      VIPRA::f3d{center.x + ((dimensions.x / 2) * cos(rotation)) -
                     ((dimensions.y / 2) * sin(rotation)),
                 center.y + ((dimensions.x / 2) * sin(rotation)) +
                     ((dimensions.y / 2) * cos(rotation))};
  const VIPRA::f3d point2 =
      VIPRA::f3d{center.x - ((dimensions.x / 2) * cos(rotation)) -
                     ((dimensions.y / 2) * sin(rotation)),
                 center.y - ((dimensions.x / 2) * sin(rotation)) +
                     ((dimensions.y / 2) * cos(rotation))};
  const VIPRA::f3d point3 =
      VIPRA::f3d{center.x - ((dimensions.x / 2) * cos(rotation)) +
                     ((dimensions.y / 2) * sin(rotation)),
                 center.y - ((dimensions.x / 2) * sin(rotation)) -
                     ((dimensions.y / 2) * cos(rotation))};
  const VIPRA::f3d point4 =
      VIPRA::f3d{center.x + ((dimensions.x / 2) * cos(rotation)) +
                     ((dimensions.y / 2) * sin(rotation)),
                 center.y + ((dimensions.x / 2) * sin(rotation)) -
                     ((dimensions.y / 2) * cos(rotation))};

  _points = {point1, point2, point3, point4};
  calc_dims();
  _area = _height * _width;
}

}  // namespace VIPRA::Geometry