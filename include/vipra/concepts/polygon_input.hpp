#pragma once

#include <type_traits>
#include <vector>

#include "vipra/concepts/module.hpp"

#include "vipra/geometry/polygon.hpp"

namespace VIPRA::Concepts {

template <typename input_t>
concept can_get_polygons = requires(input_t input) {
  { input.load_polygons() } -> std::same_as<std::vector<VIPRA::Geometry::Polygon>>;
};

template <typename input_t>
concept PolygonInput = is_module<input_t, Modules::Type::INPUT> && can_get_polygons<input_t>;
}  // namespace VIPRA::Concepts