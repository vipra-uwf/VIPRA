#pragma once

#include "vipra/types/f3d.hpp"
#include "vipra/types/size.hpp"

namespace VIPRA {
class Rectangle {
  // TODO(rolland): implement, currently axis aligned only
  // TODO(rolland): create tests
 public:
  constexpr Rectangle() = default;
  constexpr Rectangle(VIPRA::f3d point1, VIPRA::f3d point2, VIPRA::f3d point3, VIPRA::f3d point4)
      : _p1(point1), _p2(point2), _p3(point3), _p4(point4) {}
  constexpr Rectangle(VIPRA::f3d point1, VIPRA::f3d point3) : _p1(point1), _p3(point3) {
    _p2 = VIPRA::f3d(point3.x, point1.y, point1.z);
    _p4 = VIPRA::f3d(point1.x, point3.y, point3.z);
  }

  [[nodiscard]] inline constexpr auto is_point_inside(VIPRA::f3d point) const -> bool {
    return point.x >= _p1.x && point.x <= _p2.x && point.y >= _p1.y && point.y <= _p4.y;
  }

  // ---------- Getters -------------------
  [[nodiscard]] inline constexpr auto rotation() const -> VIPRA::f_pnt {
    return atan2(_p2.y - _p1.y, _p2.x - _p1.x);
  }
  [[nodiscard]] inline constexpr auto width() const -> VIPRA::f_pnt { return _p2.x - _p1.x; }
  [[nodiscard]] inline constexpr auto height() const -> VIPRA::f_pnt { return _p4.y - _p1.y; }
  [[nodiscard]] inline constexpr auto area() const -> VIPRA::f_pnt { return width() * height(); }
  [[nodiscard]] inline constexpr auto center() const -> VIPRA::f3d {
    return VIPRA::f3d(_p1.x + width() / 2, _p1.y + height() / 2, _p1.z);
  }

  [[nodiscard]] inline constexpr auto p1() const -> VIPRA::f3d { return _p1; }
  [[nodiscard]] inline constexpr auto p2() const -> VIPRA::f3d { return _p2; }
  [[nodiscard]] inline constexpr auto p3() const -> VIPRA::f3d { return _p3; }
  [[nodiscard]] inline constexpr auto p4() const -> VIPRA::f3d { return _p4; }
  [[nodiscard]] inline constexpr auto operator[](VIPRA::size idx) -> VIPRA::f3d& {
    switch (idx) {
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
  [[nodiscard]] inline constexpr auto at(VIPRA::size idx) const -> const VIPRA::f3d& {
    switch (idx) {
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
  VIPRA::f3d _p1{};
  VIPRA::f3d _p2{};
  VIPRA::f3d _p3{};
  VIPRA::f3d _p4{};
};
}  // namespace VIPRA