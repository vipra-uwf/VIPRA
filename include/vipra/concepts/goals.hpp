#pragma once

#include <concepts>
#include <type_traits>

#include "vipra/concepts/module.hpp"
#include "vipra/modules.hpp"

namespace VIPRA::Concepts {
template <typename goals_t>
concept GoalsModule = Concepts::Module<goals_t, VIPRA::Modules::Type::GOALS>;
}  // namespace VIPRA::Concepts