#pragma once

#include <vector>

#include "vipra/geometry/line.hpp"

// TODO(rolland): add in iterators

namespace VIPRA::Geometry {
struct Polygon {
  std::vector<VIPRA::Geometry::Line> edges;

  explicit Polygon(const std::vector<VIPRA::f3d>& points) {
    assert(points.size() > 2);

    for (size_t i = 0; i < points.size() - 1; ++i) {
      edges.emplace_back(points.at(i), points.at(i + 1));
    }
    edges.emplace_back(points.back(), points.front());
  }
  explicit Polygon(std::vector<VIPRA::f3d>&& points) {
    assert(points.size() > 2);

    for (size_t i = 0; i < points.size() - 1; ++i) {
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
  [[nodiscard]] inline auto is_point_inside(VIPRA::f3d point) const noexcept -> bool {
    // TODO(rolland): verify this
    bool isInside = false;
    for (const auto& edge : edges) {
      if (edge.is_point_on(point)) return true;
      if (edge.start.y > point.y != edge.end.y > point.y &&
          point.x < (edge.end.x - edge.start.x) * (point.y - edge.start.y) / (edge.end.y - edge.start.y) +
                        edge.start.x) {
        isInside = !isInside;
      }
    }
    return isInside;
  }
};
}  // namespace VIPRA::Geometry