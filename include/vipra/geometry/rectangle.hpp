#pragma once

#include <array>
#include "vipra/geometry/f3d.hpp"
#include "vipra/geometry/line.hpp"
#include "vipra/types/size.hpp"

namespace VIPRA::Geometry {
class Rectangle {
  // TODO(rolland): issue #20 implement, currently axis aligned only
  // TODO(rolland): create tests

 public:
  constexpr Rectangle(VIPRA::f3d point1, VIPRA::f3d point2, VIPRA::f3d point3, VIPRA::f3d point4)
      : _p1(point1),
        _p2(point2),
        _p3(point3),
        _p4(point4),
        _width(std::abs(point3.x - point1.x)),
        _height(std::abs(point3.y - point1.y)),
        _area(calc_area())
  {
  }
  constexpr Rectangle(VIPRA::f3d point1, VIPRA::f3d point3)
      : _p1(point1),
        _p3(point3),
        _width(std::abs(point3.x - point1.x)),
        _height(std::abs(point3.y - point1.y)),
        _area(calc_area())
  {
    _p2 = VIPRA::f3d(point3.x, point1.y, point1.z);
    _p4 = VIPRA::f3d(point1.x, point3.y, point3.z);
  }

  /**
   * @brief Checks if a point is inside the rectangle.
   *
   * @param point
   * @return true
   * @return false
   */
  [[nodiscard]] inline constexpr auto is_point_inside(VIPRA::f3d point) const noexcept -> bool
  {
    const VIPRA::f_pnt areaTri1 = triangle_area(_p1, _p2, point);
    const VIPRA::f_pnt areaTri2 = triangle_area(_p2, _p3, point);
    const VIPRA::f_pnt areaTri3 = triangle_area(_p3, _p4, point);
    const VIPRA::f_pnt areaTri4 = triangle_area(_p4, _p1, point);

    return (areaTri1 + areaTri2 + areaTri3 + areaTri4) <= area() + AREA_ERROR;
  }

  /**
   * @brief Checks if the rectangle intersects another
   *
   * @param other
   * @return true
   * @return false
   */
  [[nodiscard]] inline constexpr auto does_intersect(Rectangle const& other) const noexcept -> bool
  {
    std::array<Line, 4> lines1{Line{_p1, _p2}, {_p2, _p3}, {_p3, _p4}, {_p4, _p1}};
    std::array<Line, 4> lines2{
        Line{other._p1, other._p2}, {other._p2, other._p3}, {other._p3, other._p4}, {other._p4, other._p1}};

    for ( size_t i = 0; i < 4; i++ ) {
      for ( size_t j = 0; j < 4; j++ ) {
        if ( lines1.at(i).does_intersect(lines2.at(j)) ) return true;
      }
    }

    return false;
  }

  /**
   * @brief Checks if the rectangle intersects another
   *
   * @param other
   * @return true
   * @return false
   */
  [[nodiscard]] inline constexpr auto does_intersect(Line const& other) const noexcept -> bool
  {
    std::array<Line, 4> lines1{Line{_p1, _p2}, {_p2, _p3}, {_p3, _p4}, {_p4, _p1}};

    for ( size_t i = 0; i < 4; ++i ) {
      if ( lines1.at(i).does_intersect(other) ) return true;
    }

    return false;
  }

  // ---------- Getters -------------------
  [[nodiscard]] inline constexpr auto rotation() const noexcept -> VIPRA::f_pnt
  {
    return atan2(_p2.y - _p1.y, _p2.x - _p1.x);
  }

  [[nodiscard]] inline constexpr auto width() const noexcept -> VIPRA::f_pnt { return _width; }
  [[nodiscard]] inline constexpr auto height() const noexcept -> VIPRA::f_pnt { return _height; }

  [[nodiscard]] inline constexpr auto area() const noexcept -> VIPRA::f_pnt { return _area; }

  // TODO(rolland): issue #20 this assumes axis aligned
  [[nodiscard]] inline constexpr auto center() const noexcept -> VIPRA::f3d
  {
    return VIPRA::f3d(_p1.x + width() / 2, _p1.y + height() / 2, _p1.z);
  }

  [[nodiscard]] inline constexpr auto p1() const noexcept -> VIPRA::f3d { return _p1; }
  [[nodiscard]] inline constexpr auto p2() const noexcept -> VIPRA::f3d { return _p2; }
  [[nodiscard]] inline constexpr auto p3() const noexcept -> VIPRA::f3d { return _p3; }
  [[nodiscard]] inline constexpr auto p4() const noexcept -> VIPRA::f3d { return _p4; }
  [[nodiscard]] inline constexpr auto operator[](VIPRA::size idx) -> VIPRA::f3d&
  {
    switch ( idx ) {
      case 0:
        return _p1;
      case 1:
        return _p2;
      case 2:
        return _p3;
      case 3:
        return _p4;
      default:
        throw std::out_of_range("Attempt To Read Out of Bounds on Rect");
    }

    return _p1;
  }
  [[nodiscard]] inline constexpr auto at(VIPRA::size idx) const -> const VIPRA::f3d&
  {
    switch ( idx ) {
      case 0:
        return _p1;
      case 1:
        return _p2;
      case 2:
        return _p3;
      case 3:
        return _p4;
      default:
        throw std::out_of_range("Attempt To Read Out of Bounds on Rect");
    }

    return _p1;
  }

 private:
  VIPRA::f3d   _p1{};
  VIPRA::f3d   _p2{};
  VIPRA::f3d   _p3{};
  VIPRA::f3d   _p4{};
  VIPRA::f_pnt _width{};
  VIPRA::f_pnt _height{};
  VIPRA::f_pnt _area{};

  static constexpr VIPRA::f_pnt AREA_ERROR = 0.0001;

  /**
   * @brief Gets the area of a triangle from 3 points
   *
   * @param point1 : tri point
   * @param point2 : tri point
   * @param point3 : tri point
   * @return constexpr VIPRA::f_pnt : triangle area
   */
  [[nodiscard]] static inline constexpr auto triangle_area(VIPRA::f3d point1, VIPRA::f3d point2,
                                                           VIPRA::f3d point3) noexcept -> VIPRA::f_pnt
  {
    return std::abs((point2.x * point1.y - point1.x * point2.y) +
                    (point3.x * point2.y - point2.x * point3.y) +
                    (point1.x * point3.y - point3.x * point1.y)) /
           2;
  }

  [[nodiscard]] inline constexpr auto calc_area() const noexcept -> VIPRA::f_pnt
  {
    return (triangle_area(_p1, _p2, _p3) + triangle_area(_p1, _p3, _p4));
  }

 public:
  constexpr Rectangle() = default;
  constexpr Rectangle(Rectangle const&) = default;
  constexpr Rectangle(Rectangle&&) = default;
  constexpr auto operator=(Rectangle const&) -> Rectangle& = default;
  constexpr auto operator=(Rectangle&&) -> Rectangle& = default;
  constexpr ~Rectangle() = default;
};
}  // namespace VIPRA::Geometry