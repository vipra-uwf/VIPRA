#ifndef VIPRA_BEHAVIORS_LOCATION_HPP
#define VIPRA_BEHAVIORS_LOCATION_HPP

#include "util/class_types.hpp"
#include "vipra/geometry/f3d.hpp"
#include "vipra/randomization/distributions.hpp"
#include "vipra/randomization/random.hpp"

namespace BHVR {

/**
   * @brief A Location is an area in the map that is either predefined, such as the bathroom, or defined by a behavior created by the user.
   *     
   */
class Location {
  NON_DEFAULT_CONSTRUCTIBLE(Location)
  COPYABLE(Location)
  MOVEABLE(Location)

 public:
  constexpr explicit Location(VIPRA::f3d center, VIPRA::f3d dims) : _center(center), _dims(dims), _area(0) {
    _p1.x = center.x + (dims.x / 2);
    _p1.y = center.y + (dims.y / 2);
    _p2.x = center.x - (dims.x / 2);
    _p2.y = center.y + (dims.y / 2);
    _p3.x = center.x - (dims.x / 2);
    _p3.y = center.y - (dims.y / 2);
    _p4.x = center.x + (dims.x / 2);
    _p4.y = center.y - (dims.y / 2);
    // NOLINTNEXTLINE incorrect error
    _area = area() + AREA_ERROR;
  }

  constexpr explicit Location(VIPRA::f3d center, VIPRA::f3d dims, float rot)
      : _center(center), _dims(dims), _rot(rot), _area(0) {
    _p1.x = center.x + ((dims.x / 2) * std::cos(rot)) - ((dims.y / 2) * std::sin(rot));
    _p1.y = center.y + ((dims.x / 2) * std::sin(rot)) + ((dims.y / 2) * std::cos(rot));
    _p2.x = center.x - ((dims.x / 2) * std::cos(rot)) - ((dims.y / 2) * std::sin(rot));
    _p2.y = center.y - ((dims.x / 2) * std::sin(rot)) + ((dims.y / 2) * std::cos(rot));
    _p3.x = center.x - ((dims.x / 2) * std::cos(rot)) + ((dims.y / 2) * std::sin(rot));
    _p3.y = center.y - ((dims.x / 2) * std::sin(rot)) - ((dims.y / 2) * std::cos(rot));
    _p4.x = center.x + ((dims.x / 2) * std::cos(rot)) + ((dims.y / 2) * std::sin(rot));
    _p4.y = center.y + ((dims.x / 2) * std::sin(rot)) - ((dims.y / 2) * std::cos(rot));
    // NOLINTNEXTLINE incorrect error
    _area = area() + AREA_ERROR;
  }

  /**
   * @brief Returns the area of the rectangle
   * 
   * @return constexpr float : area
   */
  [[nodiscard]] constexpr auto area() const -> float {
    float area1 = triangle_area(_p1, _p2, _p3);
    float area2 = triangle_area(_p1, _p3, _p4);

    return (area1 += area2);
  }

  [[nodiscard]] constexpr auto center() -> VIPRA::f3d& { return _center; }
  [[nodiscard]] constexpr auto center() const -> VIPRA::f3d { return _center; }

  [[nodiscard]] constexpr auto dims() -> VIPRA::f3d& { return _dims; }
  [[nodiscard]] constexpr auto dims() const -> VIPRA::f3d { return _dims; }

  [[nodiscard]] constexpr auto rotation() -> float& { return _rot; }
  [[nodiscard]] constexpr auto rotation() const -> float { return _rot; }

  [[nodiscard]] constexpr auto random_point(VIPRA::pRNG_Engine& eng) const -> VIPRA::f3d {
    VIPRA::uniform_distribution<> wDistr{0, _dims.x};
    VIPRA::uniform_distribution<> hDistr{0, _dims.y};

    float      rotSin = std::sin(_rot);
    float      rotCos = std::cos(_rot);
    VIPRA::f3d point{wDistr(eng), hDistr(eng)};

    point.x = (point.x * rotCos - point.y * rotSin) + _center.x;
    point.y = (point.x * rotSin + point.y * rotCos) + _center.y;

    return point;
  }

  /**
   * @brief Returns if the point is inside the rectangle
   *
   * @param point : point to check
   * @return true : if inside rectangle
   * @return false : if NOT inside rectangle
   */
  [[nodiscard]] constexpr auto contains(VIPRA::f3d point) const -> bool {
    float areaTri1 = triangle_area(_p1, _p2, point);
    float areaTri2 = triangle_area(_p2, _p3, point);
    float areaTri3 = triangle_area(_p3, _p4, point);
    float areaTri4 = triangle_area(_p4, _p1, point);

    return (areaTri1 += areaTri2 += areaTri3 += areaTri4) <= _area;
  }

  void set_center(VIPRA::f3d center) {
    _center = center;
    _p1.x = center.x + (_dims.x / 2);
    _p1.y = center.y + (_dims.y / 2);
    _p2.x = center.x - (_dims.x / 2);
    _p2.y = center.y + (_dims.y / 2);
    _p3.x = center.x - (_dims.x / 2);
    _p3.y = center.y - (_dims.y / 2);
    _p4.x = center.x + (_dims.x / 2);
    _p4.y = center.y - (_dims.y / 2);
  }

  void set_dims(VIPRA::f3d dims) {
    _dims = dims;
    _p1.x = _center.x + (_dims.x / 2);
    _p1.y = _center.y + (_dims.y / 2);
    _p2.x = _center.x - (_dims.x / 2);
    _p2.y = _center.y + (_dims.y / 2);
    _p3.x = _center.x - (_dims.x / 2);
    _p3.y = _center.y - (_dims.y / 2);
    _p4.x = _center.x + (_dims.x / 2);
    _p4.y = _center.y - (_dims.y / 2);
    // NOLINTNEXTLINE incorrect error
    _area = area() + AREA_ERROR;
  }

 private:
  VIPRA::f3d _p1;
  VIPRA::f3d _p2;
  VIPRA::f3d _p3;
  VIPRA::f3d _p4;
  VIPRA::f3d _center;
  VIPRA::f3d _dims;
  float      _rot{};
  float      _area;

  static constexpr float AREA_ERROR = 0.0001;

  /**
   * @brief Gets the area of a triangle from 3 points
   * 
   * @param point1 : tri point 
   * @param point2 : tri point 
   * @param point3 : tri point 
   * @return constexpr float : triangle area
   */
  [[nodiscard]] static constexpr auto triangle_area(VIPRA::f3d point1, VIPRA::f3d point2, VIPRA::f3d point3)
      -> float {
    return std::abs((point2.x * point1.y - point1.x * point2.y) +
                    (point3.x * point2.y - point2.x * point3.y) +
                    (point1.x * point3.y - point3.x * point1.y)) /
           2;
  }
};
}  //namespace BHVR
#endif