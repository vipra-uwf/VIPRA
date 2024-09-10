#pragma once

#include <algorithm>
#include <vector>

#include "vipra/geometry/circle.hpp"
#include "vipra/geometry/line.hpp"

// TODO(rolland): issue #19 add in iterators
namespace VIPRA::Geometry {
struct Polygon {
  std::vector<VIPRA::Geometry::Line> edges;

  explicit Polygon(const std::vector<VIPRA::f3d>& points)
  {
    assert(points.size() > 2);

    for ( size_t i = 0; i < points.size() - 1; ++i ) {
      edges.emplace_back(points.at(i), points.at(i + 1));
    }
    edges.emplace_back(points.back(), points.front());
  }

  explicit Polygon(std::vector<VIPRA::f3d>&& points)
  {
    assert(points.size() > 2);

    for ( size_t i = 0; i < points.size() - 1; ++i ) {
      edges.emplace_back(points.at(i), points.at(i + 1));
    }
    edges.emplace_back(points.back(), points.front());
  }

  /**
   * @brief Checks if a point is inside the polygon.
   * 
   * @param point 
   * @return true 
   * @return false 
   */
  [[nodiscard]] VIPRA_INLINE auto is_point_inside(VIPRA::f3d point) const noexcept -> bool
  {
    // TODO(rolland): verify this
    bool isInside = false;
    for ( auto const& edge : edges ) {
      if ( edge.is_point_on(point) ) return true;
      if ( edge.start.y > point.y != edge.end.y > point.y &&
           point.x < (edge.end.x - edge.start.x) * (point.y - edge.start.y) / (edge.end.y - edge.start.y) +
                         edge.start.x ) {
        isInside = ! isInside;
      }
    }
    return isInside;
  }

  /**
   * @brief Checks if the polygon intersects with the given circle
   * 
   * @param circle 
   * @return true 
   * @return false 
   */
  [[nodiscard]] VIPRA_INLINE auto does_intersect(VIPRA::Geometry::Circle const& circle) const noexcept -> bool
  {
    return std::any_of(edges.begin(), edges.end(),
                       [&](auto const& edge) { return circle.does_intersect(edge); });
  }

  /**
   * @brief Returns the geometric center of the polygon
   * 
   * @return VIPRA::f3d 
   */
  [[nodiscard]] VIPRA_INLINE auto center() const noexcept -> VIPRA::f3d
  {
    VIPRA::f3d center;
    std::for_each(edges.begin(), edges.end(),
                  [&](VIPRA::Geometry::Line const& edge) { center += edge.start; });

    return center /= edges.size();
  }

  [[nodiscard]] VIPRA_INLINE auto random_point() const noexcept -> VIPRA::f3d
  {
    // TODO(rolland, issue #19): implement a proper random_point method
    return center();
  }

  Polygon() = default;
  Polygon(Polygon const&) = default;
  Polygon(Polygon&&) noexcept = default;
  auto operator=(Polygon const&) -> Polygon& = default;
  auto operator=(Polygon&&) noexcept -> Polygon& = default;
};
}  // namespace VIPRA::Geometry