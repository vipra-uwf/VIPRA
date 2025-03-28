#pragma once

#include <concepts>
#include <vector>

#include "vipra/geometry/circle.hpp"
#include "vipra/geometry/f3d.hpp"
#include "vipra/geometry/line.hpp"
#include "vipra/geometry/rectangle.hpp"
#include "vipra/geometry/triangle.hpp"

namespace VIPRA::Geometry {

template <typename class_t>
concept has_sides = requires(class_t instance) {
  { instance.sides() };
};

template <has_sides polygon_t, has_sides other_t>
VIPRA_POLY_FUNC auto do_intersect(polygon_t const& polygon,
                                  other_t const&   other) noexcept -> bool
{
  auto const sideList = polygon.sides();
  return std::any_of(sideList.begin(), sideList.end(), [&](Line const& side) {
    auto const otherSides = other.sides();
    return std::any_of(otherSides.begin(), otherSides.end(), [&](Line const& otherSide) {
      return otherSide.does_intersect(side);
    });
  });
}

template <has_sides polygon_t>
VIPRA_POLY_FUNC auto do_intersect(polygon_t const& polygon,
                                  Line const&      line) noexcept -> bool
{
  auto const sideList = polygon.sides();
  return std::any_of(sideList.begin(), sideList.end(),
                     [&](Line const& side) { return line.does_intersect(side); });
}

template <has_sides polygon_t>
VIPRA_POLY_FUNC auto do_intersect(Line const&      line,
                                  polygon_t const& polygon) noexcept -> bool
{
  auto const sideList = polygon.sides();
  return std::any_of(sideList.begin(), sideList.end(),
                     [&](Line const& side) { return line.does_intersect(side); });
}

template <has_sides polygon_t>
VIPRA_POLY_FUNC auto do_intersect(polygon_t const& polygon,
                                  Circle const&    circle) noexcept -> bool
{
  auto const sideList = polygon.sides();
  return std::any_of(sideList.begin(), sideList.end(),
                     [&](Line const& side) { return circle.does_intersect(side); });
}

template <has_sides polygon_t>
VIPRA_POLY_FUNC auto do_intersect(Circle const&    circle,
                                  polygon_t const& polygon) noexcept -> bool
{
  auto const sideList = polygon.sides();
  return std::any_of(sideList.begin(), sideList.end(),
                     [&](Line const& side) { return circle.does_intersect(side); });
}

}  // namespace VIPRA::Geometry