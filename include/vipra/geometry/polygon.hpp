#pragma once

#include <array>
#include <cassert>
#include <cstddef>
#include <limits>
#include <random>
#include <vector>

#include "vipra/geometry/f3d.hpp"
#include "vipra/geometry/line.hpp"

#include "vipra/geometry/rectangle.hpp"
#include "vipra/geometry/triangle.hpp"
#include "vipra/random/random.hpp"
namespace VIPRA::Geometry {

class Polygon {
 public:
  auto random_point(VIPRA::Random::Engine& engine) const noexcept -> f3d;

  [[nodiscard]] auto is_point_inside(f3d point) const noexcept -> bool;
  [[nodiscard]] auto bounding_box() const noexcept -> Rectangle;
  [[nodiscard]] auto center() const noexcept -> f3d;
  [[nodiscard]] auto points() const noexcept -> std::vector<f3d> const&
  {
    return _points;
  }
  [[nodiscard]] auto sides() const noexcept -> std::vector<Line>;

 private:
  std::vector<f3d> _points;

 public:
  explicit Polygon(std::vector<f3d> const& points) : _points(points) {}

  explicit Polygon(std::vector<f3d>&& points) : _points(std::move(points)) {}

  template <size_t point_s>
  explicit Polygon(std::array<f3d, point_s> const& points)
  {
    _points.resize(point_s);
    std::copy(points.begin(), points.end(), _points.begin());
  }

  template <size_t point_s>
  explicit Polygon(std::array<f3d, point_s>&& points)
  {
    _points.resize(point_s);
    std::copy(points.begin(), points.end(), _points.begin());
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

inline auto Polygon::is_point_inside(f3d point) const noexcept -> bool
{
  if ( _points.size() < 3 ) return false;  // A polygon must have at least 3 vertices

  bool inside = false;
  for ( size_t i = 0, j = _points.size() - 1; i < _points.size(); j = i++ ) {
    f_pnt xi = _points[i].x;
    f_pnt yi = _points[i].y;
    f_pnt xj = _points[j].x;
    f_pnt yj = _points[j].y;

    // Check if the point is on an edge
    if ( (yi > point.y) != (yj > point.y) ) {
      f_pnt intersectX = (xj - xi) * (point.y - yi) / (yj - yi) + xi;
      if ( point.x < intersectX ) inside = ! inside;
    }
  }
  return inside;
}

inline auto Polygon::center() const noexcept -> f3d
{
  const auto sideList = sides();
  f3d        center;

  std::for_each(sideList.begin(), sideList.end(),
                [&](Line const& edge) { center += edge.start; });

  return center /= sideList.size();
}

inline auto Polygon::random_point(VIPRA::Random::Engine& engine) const noexcept -> f3d
{
  // TODO(rolland): this is disgusting

  auto box = bounding_box();

  std::uniform_real_distribution<VIPRA::f_pnt> xDist{box.sides()[3].end.x,
                                                     box.sides()[3].start.x};
  std::uniform_real_distribution<VIPRA::f_pnt> yDist{box.sides()[2].end.y,
                                                     box.sides()[2].start.y};

  f3d point;

  do {
    point.x = xDist(engine);
    point.y = yDist(engine);
  } while ( ! box.is_point_inside(point) );
  return point;
}

inline auto Polygon::bounding_box() const noexcept -> Rectangle
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

  const f3d topLeft = f3d{botLeft.x, topRight.y};
  const f3d botRight = f3d{topRight.x, botLeft.y};

  return Rectangle{botLeft, topLeft, topRight, botRight};
}

inline auto Polygon::sides() const noexcept -> std::vector<Line>
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