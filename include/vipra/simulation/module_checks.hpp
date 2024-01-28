#pragma once

#include "vipra/concepts/goals.hpp"
#include "vipra/concepts/has_type.hpp"
#include "vipra/concepts/input.hpp"
#include "vipra/concepts/map.hpp"
#include "vipra/concepts/model.hpp"
#include "vipra/concepts/obstacle_set.hpp"
#include "vipra/concepts/output.hpp"
#include "vipra/concepts/output_coordinator.hpp"
#include "vipra/concepts/parameters.hpp"
#include "vipra/concepts/pedset.hpp"

namespace VIPRA::Checks {
template <typename type_t>
struct Params {
  static_assert(Concepts::has_type<type_t>::value, "Missing Module Type");
  // NOLINTNEXTLINE(readability-identifier-naming) lowercase is a regular naming convetion here
  static constexpr bool value = Concepts::ParamModule<type_t>;
};

template <typename type_t>
struct Input {
  static_assert(Concepts::has_type<type_t>::value, "Missing Module Type");
  // NOLINTNEXTLINE(readability-identifier-naming) lowercase is a regular naming convetion here
  static constexpr bool value = Concepts::InputModule<type_t>;
};

template <typename type_t>
struct OutputCoord {
  static_assert(Concepts::has_type<type_t>::value, "Missing Module Type");
  // NOLINTNEXTLINE(readability-identifier-naming) lowercase is a regular naming convetion here
  static constexpr bool value = Concepts::OutputCoordinator<type_t>;
};

template <typename type_t>
struct Model {
  static_assert(Concepts::has_type<type_t>::value, "Missing Module Type");
  // NOLINTNEXTLINE(readability-identifier-naming) lowercase is a regular naming convetion here
  static constexpr bool value = Concepts::ModelModule<type_t>;
};

template <typename type_t>
struct Pedset {
  static_assert(Concepts::has_type<type_t>::value, "Missing Module Type");
  // NOLINTNEXTLINE(readability-identifier-naming) lowercase is a regular naming convetion here
  static constexpr bool value = Concepts::PedsetModule<type_t>;
};

template <typename type_t>
struct Goals {
  static_assert(Concepts::has_type<type_t>::value, "Missing Module Type");
  // NOLINTNEXTLINE(readability-identifier-naming) lowercase is a regular naming convetion here
  static constexpr bool value = Concepts::GoalsModule<type_t>;
};

template <typename type_t>
struct Map {
  static_assert(Concepts::has_type<type_t>::value, "Missing Module Type");
  // NOLINTNEXTLINE(readability-identifier-naming) lowercase is a regular naming convetion here
  static constexpr bool value = Concepts::MapModule<type_t>;
};
}  // namespace VIPRA::Checks