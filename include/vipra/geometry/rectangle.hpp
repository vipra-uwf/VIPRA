#pragma once

#include "vipra/geometry/f3d.hpp"
#include "vipra/geometry/polygon.hpp"
#include "vipra/geometry/triangle.hpp"
#include "vipra/types/float.hpp"
namespace VIPRA::Geometry {
class Rectangle : public PolygonBase<4> {
 public:
  static constexpr VIPRA::f_pnt AREA_ERROR = 0.0001;

  constexpr Rectangle() = default;

  // NOLINTNEXTLINE(google-explicit-constructor)
  Rectangle(PolygonBase<4> const& polygon)
      : PolygonBase<4>(polygon.sides()),
        _area(calc_area()),
        _center((_sides[0].start + _sides[0].end + _sides[1].start +
                 _sides[1].end) /
                4)
  {
  }

  constexpr Rectangle(VIPRA::f3d const& center, VIPRA::f3d const& dimensions,
                      VIPRA::f_pnt rotation)
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

    _sides[0] = Line{point1, point2};
    _sides[1] = Line{point2, point3};
    _sides[2] = Line{point3, point4};
    _sides[3] = Line{point4, point1};

    _area = calc_area();
  }

  constexpr Rectangle(VIPRA::f3d const& point1, VIPRA::f3d const& point2,
                      VIPRA::f3d const& point3, VIPRA::f3d const& point4)
      : PolygonBase<4>(std::array{Line{point1, point2}, Line{point2, point3},
                                  Line{point3, point4}, Line{point4, point1}}),
        _area(calc_area()),
        _center((_sides[0].start + _sides[0].end + _sides[1].start +
                 _sides[1].end) /
                4)
  {
  }
  explicit Rectangle(std::vector<VIPRA::f3d> const& points)
      : PolygonBase<4>(points),
        _area(calc_area()),
        _center((_sides[0].start + _sides[0].end + _sides[1].start +
                 _sides[1].end) /
                4)
  {
  }
  explicit Rectangle(std::vector<VIPRA::Geometry::Line> const& sides)
      : PolygonBase<4>(sides),
        _area(calc_area()),
        _center((_sides[0].start + _sides[0].end + _sides[1].start +
                 _sides[1].end) /
                4)
  {
  }

  POLY_FUNC auto is_point_inside(VIPRA::f3d point) const noexcept -> bool
  {
    const VIPRA::f_pnt areaTri1 =
        triangle_area(_sides[0].start, _sides[0].end, point);
    const VIPRA::f_pnt areaTri2 =
        triangle_area(_sides[1].start, _sides[1].end, point);
    const VIPRA::f_pnt areaTri3 =
        triangle_area(_sides[2].start, _sides[2].end, point);
    const VIPRA::f_pnt areaTri4 =
        triangle_area(_sides[3].start, _sides[3].end, point);

    return (areaTri1 + areaTri2 + areaTri3 + areaTri4) <= area() + AREA_ERROR;
  }

  POLY_FUNC auto rotation() const noexcept -> VIPRA::f_pnt
  {
    return atan2(_sides[0].end.y - _sides[0].start.y,
                 _sides[0].end.x - _sides[0].start.x);
  }

  POLY_FUNC auto width() const noexcept -> VIPRA::f_pnt
  {
    return _sides[0].length();
  }
  POLY_FUNC auto height() const noexcept -> VIPRA::f_pnt
  {
    return _sides[1].length();
  }
  POLY_FUNC auto area() const noexcept -> VIPRA::f_pnt { return _area; }

  POLY_FUNC auto center() const noexcept -> VIPRA::f3d const&
  {
    return _center;
  }

 private:
  VIPRA::f_pnt _area{};
  VIPRA::f3d   _center;

  [[nodiscard]] inline constexpr auto calc_area() const noexcept -> VIPRA::f_pnt
  {
    return (triangle_area(_sides[0].start, _sides[0].end, _sides[1].start) +
            triangle_area(_sides[0].start, _sides[1].end, _sides[3].start));
  }

  [[nodiscard]] static inline constexpr auto triangle_area(
      VIPRA::f3d point1, VIPRA::f3d point2,
      VIPRA::f3d point3) noexcept -> VIPRA::f_pnt
  {
    return std::abs((point2.x * point1.y - point1.x * point2.y) +
                    (point3.x * point2.y - point2.x * point3.y) +
                    (point1.x * point3.y - point3.x * point1.y)) /
           2;
  }
};
}  // namespace VIPRA::Geometry