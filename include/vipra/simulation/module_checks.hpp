#pragma once

#include "vipra/modules/goals.hpp"
#include "vipra/modules/input.hpp"
#include "vipra/modules/model.hpp"
#include "vipra/modules/obstacles.hpp"
#include "vipra/modules/pedestrians.hpp"
#include "vipra/special_modules/output.hpp"
#include "vipra/util/template_specialization.hpp"

namespace VIPRA::Checks {
template <typename type_t>
struct Input {
  // NOLINTNEXTLINE(readability-identifier-naming) lowercase is a regular naming convetion here
  static constexpr bool value = std::derived_from<type_t, VIPRA::Modules::Input<type_t>>;
};

template <typename type_t>
struct OutputCoord {
  // NOLINTNEXTLINE(readability-identifier-naming) lowercase is a regular naming convetion here
  static constexpr bool value = Util::is_specialization<type_t, VIPRA::CoordModules::Output>::value;
};

template <typename type_t>
struct Model {
  // NOLINTNEXTLINE(readability-identifier-naming) lowercase is a regular naming convetion here
  static constexpr bool value = std::derived_from<type_t, VIPRA::Modules::Model<type_t>>;
};

template <typename type_t>
struct Pedset {
  // NOLINTNEXTLINE(readability-identifier-naming) lowercase is a regular naming convetion here
  static constexpr bool value = std::derived_from<type_t, VIPRA::Modules::Pedestrians<type_t>>;
};

template <typename type_t>
struct Goals {
  // NOLINTNEXTLINE(readability-identifier-naming) lowercase is a regular naming convetion here
  static constexpr bool value = std::derived_from<type_t, VIPRA::Modules::Goals<type_t>>;
};

template <typename type_t>
struct Obstacles {
  // NOLINTNEXTLINE(readability-identifier-naming) lowercase is a regular naming convetion here
  static constexpr bool value = std::derived_from<type_t, VIPRA::Modules::Obstacles<type_t>>;
};
}  // namespace VIPRA::Checks