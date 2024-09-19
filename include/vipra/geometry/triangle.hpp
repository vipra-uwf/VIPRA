#pragma once

#include "vipra/geometry/f3d.hpp"

#include "vipra/geometry/polygon.hpp"

namespace VIPRA::Geometry {
class Triangle : public PolygonBase<3> {
 public:
  constexpr explicit Triangle(VIPRA::f3d point1, VIPRA::f3d point2, VIPRA::f3d point3)
      : PolygonBase<3>(std::array{point1, point2, point3})
  {
  }

  POLY_FUNC auto area() const noexcept -> VIPRA::f_pnt
  {
    return std::abs((_sides[0].end.x * _sides[0].start.y - _sides[0].start.x * _sides[0].end.y) +
                    (_sides[1].end.x * _sides[0].end.y - _sides[0].end.x * _sides[1].end.y) +
                    (_sides[0].start.x * _sides[1].end.y - _sides[1].end.x * _sides[0].start.y)) /
           2;
  }
};
}  // namespace VIPRA::Geometry