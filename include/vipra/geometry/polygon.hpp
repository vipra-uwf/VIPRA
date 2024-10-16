#pragma once

#include <array>
#include <cassert>
#include <cstddef>
#include <limits>
#include <vector>

#include "vipra/geometry/f3d.hpp"
#include "vipra/geometry/line.hpp"

#include "vipra/geometry/rectangle.hpp"
#include "vipra/geometry/triangle.hpp"
#include "vipra/random/random.hpp"
namespace VIPRA::Geometry {

class Polygon {
 public:
  VIPRA_POLY_FUNC auto random_point(
      VIPRA::Random::Engine& engine) const noexcept -> f3d;

  VIPRA_POLY_FUNC auto is_point_inside(f3d point) const noexcept -> bool;
  VIPRA_POLY_FUNC auto bounding_box() const noexcept -> Rectangle;
  VIPRA_POLY_FUNC auto center() const noexcept -> f3d;
  VIPRA_POLY_FUNC auto points() const noexcept -> std::vector<f3d> const&
  {
    return _points;
  }

  VIPRA_POLY_FUNC auto sides() const noexcept -> std::vector<Line>;

 private:
  std::vector<f3d> _points;

 public:
  explicit Polygon(std::vector<f3d> const& points) : _points(points) {}

  explicit Polygon(std::vector<f3d>&& points) : _points(std::move(points)) {}

  template <size_t point_s>
  explicit Polygon(std::array<f3d, point_s> const& points) : _points(points)
  {
  }

  template <size_t point_s>
  explicit Polygon(std::array<f3d, point_s>&& points)
      : _points(std::move(points))
  {
  }

  explicit Polygon(std::vector<Line> const& lines)
  {
    _points.resize(lines.size());
    for ( size_t i = 0; i < lines.size(); ++i ) {
      _points[i] = lines[i].start;
    }
  }

  explicit Polygon(std::vector<Line>&& lines)
  {
    _points.resize(lines.size());
    for ( size_t i = 0; i < lines.size(); ++i ) {
      _points[i] = lines[i].start;
    }
  }

  template <size_t point_s>
  explicit Polygon(std::array<Line, point_s> const& lines)
  {
    _points.resize(point_s);
    for ( size_t i = 0; i < point_s; ++i ) {
      _points[i] = lines[i].start;
    }
  }

  template <size_t point_s>
  explicit Polygon(std::array<Line, point_s>&& lines)
  {
    _points.resize(point_s);
    for ( size_t i = 0; i < point_s; ++i ) {
      _points[i] = lines[i].start;
    }
  }

  ~Polygon() = default;
  Polygon() = default;
  Polygon(Polygon const&) = default;
  auto operator=(Polygon const&) -> Polygon& = default;
  Polygon(Polygon&&) noexcept = default;
  auto operator=(Polygon&&) noexcept -> Polygon& = default;
};

// ---------------------------------- IMPLEMENTATION ------------------------------------------------

constexpr auto Polygon::is_point_inside(f3d point) const noexcept -> bool
{
  // TODO(rolland): verify this
  bool isInside = false;
  for ( auto const& side : sides() ) {
    if ( side.is_point_on(point) ) return true;
    if ( side.start.y > point.y != side.end.y > point.y &&
         point.x < (side.end.x - side.start.x) * (point.y - side.start.y) /
                           (side.end.y - side.start.y) +
                       side.start.x ) {
      isInside = ! isInside;
    }
  }
  return isInside;
}

VIPRA_POLY_FUNC auto Polygon::center() const noexcept -> f3d
{
  const auto sideList = sides();
  f3d        center;

  std::for_each(sideList.begin(), sideList.end(),
                [&](Line const& edge) { center += edge.start; });

  return center /= sideList.size();
}

VIPRA_POLY_FUNC auto Polygon::random_point(
    VIPRA::Random::Engine& engine) const noexcept -> f3d
{
  // TODO(rolland): this is disgusting

  auto box = bounding_box();

  Random::uniform_distribution<VIPRA::f_pnt> xDist{box.sides()[3].end.x,
                                                   box.sides()[3].start.x};
  Random::uniform_distribution<VIPRA::f_pnt> yDist{box.sides()[2].end.y,
                                                   box.sides()[2].start.y};

  f3d point;

  do {
    point.x = xDist(engine);
    point.y = yDist(engine);
  } while ( ! box.is_point_inside(point) );
  return point;
}

VIPRA_POLY_FUNC auto Polygon::bounding_box() const noexcept -> Rectangle
{
  f3d botLeft{std::numeric_limits<VIPRA::f_pnt>::max(),
              std::numeric_limits<VIPRA::f_pnt>::max()};
  f3d topRight{std::numeric_limits<VIPRA::f_pnt>::min(),
               std::numeric_limits<VIPRA::f_pnt>::min()};
  for ( auto const& point : _points ) {
    topRight.x = std::max({topRight.x, point.x});
    topRight.y = std::max({topRight.y, point.y});
    botLeft.x = std::min({botLeft.x, point.x});
    botLeft.y = std::min({botLeft.y, point.y});
  }

  f3d topLeft = f3d{botLeft.x, topRight.y};
  f3d botRight = f3d{topRight.x, botLeft.y};

  return Rectangle{botLeft, topLeft, topRight, botRight};
}

VIPRA_POLY_FUNC auto Polygon::sides() const noexcept -> std::vector<Line>
{
  std::vector<Line> lines;
  lines.resize(_points.size());

  for ( size_t i = 0; i < _points.size() - 1; ++i ) {
    lines[i] = Line{_points[i], _points[i + 1]};
  }
  lines.back() = Line{_points.back(), _points.front()};

  return lines;
}

}  // namespace VIPRA::Geometry