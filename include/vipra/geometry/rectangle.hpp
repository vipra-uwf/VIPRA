#pragma once

#include "vipra/geometry/polygon.hpp"
#include "vipra/geometry/triangle.hpp"
namespace VIPRA::Geometry {
class Rectangle : public PolygonBase<4> {
 public:
  static constexpr VIPRA::f_pnt AREA_ERROR = 0.0001;

  constexpr Rectangle() = default;
  constexpr Rectangle(VIPRA::f3d const& point1, VIPRA::f3d const& point2, VIPRA::f3d const& point3,
                      VIPRA::f3d const& point4)
      : PolygonBase<4>(std::array{Line{point1, point2}, Line{point2, point3}, Line{point3, point4},
                                  Line{point4, point1}})
  {
  }
  explicit Rectangle(std::vector<VIPRA::f3d> const& points) : PolygonBase<4>(points) {}
  explicit Rectangle(std::vector<VIPRA::Geometry::Line> const& sides) : PolygonBase<4>(sides) {}

  POLY_FUNC auto is_point_inside(VIPRA::f3d point) const noexcept -> bool
  {
    const VIPRA::f_pnt areaTri1 = Triangle{_sides[0].start, _sides[0].end, point}.area();
    const VIPRA::f_pnt areaTri2 = Triangle{_sides[1].start, _sides[1].end, point}.area();
    const VIPRA::f_pnt areaTri3 = Triangle{_sides[2].start, _sides[2].end, point}.area();
    const VIPRA::f_pnt areaTri4 = Triangle{_sides[3].start, _sides[3].end, point}.area();

    return (areaTri1 + areaTri2 + areaTri3 + areaTri4) <= area() + AREA_ERROR;
  }

  POLY_FUNC auto rotation() const noexcept -> VIPRA::f_pnt
  {
    return atan2(_sides[0].end.y - _sides[0].start.y, _sides[0].end.x - _sides[0].start.x);
  }

  POLY_FUNC auto triangularize() const noexcept -> std::vector<PolygonBase<3>>
  {
    return {
        PolygonBase<3>{std::array{_sides[0], Line{_sides[0].start, center()}, Line{_sides[0].end, center()}}},
        PolygonBase<3>{std::array{_sides[1], Line{_sides[1].start, center()}, Line{_sides[1].end, center()}}},
        PolygonBase<3>{std::array{_sides[2], Line{_sides[2].start, center()}, Line{_sides[2].end, center()}}},
        PolygonBase<3>{
            std::array{_sides[3], Line{_sides[3].start, center()}, Line{_sides[3].end, center()}}}};
  }

  POLY_FUNC auto width() const noexcept -> VIPRA::f_pnt { return _sides[0].length(); }
  POLY_FUNC auto height() const noexcept -> VIPRA::f_pnt { return _sides[1].length(); }
  POLY_FUNC auto area() const noexcept -> VIPRA::f_pnt { return width() * height(); }
};
}  // namespace VIPRA::Geometry