#pragma once

#include <optional>
#include <type_traits>
#include <vector>

#include "vipra/concepts/module.hpp"

#include "vipra/geometry/polygon.hpp"

namespace VIPRA::Concepts {

template <typename input_t>
concept can_get_polygons = requires(input_t input) {
  { input.load_polygons("key") } -> std::same_as<std::optional<std::vector<VIPRA::Geometry::Polygon>>>;
};

template <typename input_t>
concept PolygonInput = is_type<input_t, Modules::Type::INPUT> && can_get_polygons<input_t>;

class DummyPolygonInput {
  // NOLINTBEGIN
 public:
  constexpr static VIPRA::Modules::Type _VIPRA_MODULE_TYPE_ = VIPRA::Modules::Type::INPUT;

  auto load_polygons(std::string_view /*unused*/) const
      -> std::optional<std::vector<VIPRA::Geometry::Polygon>> {
    return std::nullopt;
  }
  // NOLINTEND
};

static_assert(PolygonInput<DummyPolygonInput>);
}  // namespace VIPRA::Concepts