#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <limits>
#include <type_traits>

#include "vipra/geometry/circle.hpp"
#include "vipra/geometry/line.hpp"

#include "vipra/logging/logging.hpp"
#include "vipra/macros/performance.hpp"
#include "vipra/random/distributions.hpp"
#include "vipra/random/random.hpp"
#include "vipra/types/float.hpp"
#include "vipra/util/template_specialization.hpp"
namespace VIPRA::Geometry {

#define POLY_FUNC [[nodiscard]] VIPRA_PERF_FUNC constexpr
#define POLY_FUNC_W_DISCARD VIPRA_PERF_FUNC constexpr

template <size_t side_s,
          typename container_t = std::array<VIPRA::Geometry::Line, side_s>>
class PolygonBase {
  static_assert(side_s > 2);

 public:
  POLY_FUNC auto is_point_inside(VIPRA::f3d point) const noexcept -> bool
  {
    // TODO(rolland): verify this
    bool isInside = false;
    for ( auto const& side : _sides ) {
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

  POLY_FUNC auto does_intersect(PolygonBase const& other) const noexcept -> bool
  {
    return std::any_of(_sides.begin(), _sides.end(), [&](auto const& side) {
      return other.does_intersect(side);
    });
  }

  POLY_FUNC
  auto does_intersect(Line const& line) const noexcept -> bool
  {
    return std::any_of(_sides.begin(), _sides.end(), [&](auto const& side) {
      return line.does_intersect(side);
    });
  }
  POLY_FUNC auto does_intersect(
      VIPRA::Geometry::Circle const& circle) const noexcept -> bool
  {
    return std::any_of(_sides.begin(), _sides.end(), [&](auto const& edge) {
      return circle.does_intersect(edge);
    });
  }

  POLY_FUNC auto center() const noexcept -> VIPRA::f3d
  {
    VIPRA::f3d center;
    std::for_each(
        _sides.begin(), _sides.end(),
        [&](VIPRA::Geometry::Line const& edge) { center += edge.start; });

    return center /= _sides.size();
  }

  POLY_FUNC auto sides() const noexcept -> container_t const& { return _sides; }

  POLY_FUNC auto random_point(VIPRA::Random::Engine& engine) const noexcept
      -> VIPRA::f3d
  {
    // TODO(rolland): this is disgusting

    auto box = bounding_box();

    Random::uniform_distribution<VIPRA::f_pnt> xDist{box.sides()[3].end.x,
                                                     box.sides()[3].start.x};
    Random::uniform_distribution<VIPRA::f_pnt> yDist{box.sides()[2].end.y,
                                                     box.sides()[2].start.y};

    VIPRA::f3d point;

    do {
      point.x = xDist(engine);
      point.y = yDist(engine);
    } while ( ! box.is_point_inside(point) );
    return point;
  }

  POLY_FUNC auto bounding_box() const noexcept -> PolygonBase<4>
  {
    VIPRA::f3d botLeft{std::numeric_limits<VIPRA::f_pnt>::max(),
                       std::numeric_limits<VIPRA::f_pnt>::max()};
    VIPRA::f3d topRight{std::numeric_limits<VIPRA::f_pnt>::min(),
                        std::numeric_limits<VIPRA::f_pnt>::min()};
    for ( auto const& side : _sides ) {
      topRight.x = std::max({topRight.x, side.start.x, side.end.x});
      topRight.y = std::max({topRight.y, side.start.y, side.end.y});
      botLeft.x = std::min({botLeft.x, side.start.x, side.end.x});
      botLeft.y = std::min({botLeft.y, side.start.y, side.end.y});
    }

    VIPRA::f3d topLeft = VIPRA::f3d{botLeft.x, topRight.y};
    VIPRA::f3d botRight = VIPRA::f3d{topRight.x, botLeft.y};

    return PolygonBase<4>{std::array{botLeft, topLeft, topRight, botRight}};
  }

 protected:
  // NOLINTNEXTLINE(cppcoreguidelines-non-private-member-variables-in-classes, misc-non-private-member-variables-in-classes)
  container_t _sides;

 public:
  template <size_t point_s>
  constexpr explicit PolygonBase(std::array<VIPRA::f3d, point_s> const& points)
  {
    static_assert(side_s == point_s);

    for ( size_t i = 0; i < point_s - 1; ++i ) {
      _sides[i] = Line(points.at(i), points.at(i + 1));
    }

    _sides[side_s - 1] = Line(points.back(), points.front());
  }

  explicit PolygonBase(std::vector<VIPRA::f3d> const& points)
  {
    assert((points.size() == side_s) ||
           side_s == std::numeric_limits<size_t>::max());

    if constexpr ( Util::is_specialization<container_t, std::vector>::value ) {
      for ( size_t i = 0; i < points.size() - 1; ++i ) {
        _sides.emplace_back(points.at(i), points.at(i + 1));
      }
      _sides.emplace_back(points.back(), points.front());
    }
    else {
      for ( size_t i = 0; i < points.size() - 1; ++i ) {
        _sides[i / 2] = Line(points.at(i), points.at(i + 1));
      }
      _sides[side_s - 1] = Line(points.back(), points.front());
    }
  }

  template <typename = typename std::enable_if<std::is_same_v<
                container_t, std::array<VIPRA::Geometry::Line, side_s>>>>
  explicit PolygonBase(std::vector<VIPRA::Geometry::Line> const& sides)
  {
    assert(sides.size() == side_s);
    std::copy(sides.begin(), sides.end(), _sides.begin());
  }
  template <typename = typename std::enable_if<std::is_same_v<
                container_t, std::array<VIPRA::Geometry::Line, side_s>>>>
  explicit PolygonBase(std::vector<VIPRA::Geometry::Line>&& sides) noexcept
  {
    assert(sides.size() == side_s);
    std::move(sides.begin(), sides.end(), _sides.begin());
  }

  constexpr explicit PolygonBase(container_t const& sides) : _sides(sides) {}
  constexpr explicit PolygonBase(container_t&& sides) noexcept : _sides(sides)
  {
  }
  ~PolygonBase() = default;
  constexpr PolygonBase() = default;
  constexpr PolygonBase(PolygonBase const&) = default;
  constexpr auto operator=(PolygonBase const&) -> PolygonBase& = default;
  constexpr PolygonBase(PolygonBase&&) noexcept = default;
  constexpr auto operator=(PolygonBase&&) noexcept -> PolygonBase& = default;
};

using Polygon = PolygonBase<std::numeric_limits<size_t>::max(),
                            std::vector<VIPRA::Geometry::Line>>;
static constexpr size_t ANY = std::numeric_limits<size_t>::max();

}  // namespace VIPRA::Geometry