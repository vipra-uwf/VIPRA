#pragma once

#include <concepts>
#include <optional>
#include <string_view>
#include <type_traits>

#include "vipra/concepts/module.hpp"
#include "vipra/modules.hpp"

namespace VIPRA::Concepts {
template <typename input_t>
concept InputModule = is_type<input_t, Modules::Type::INPUT> && requires(input_t input) {
  input_t::MODULE_TYPE == VIPRA::Modules::Type::INPUT;
  std::constructible_from<input_t, std::string_view>;
  { input.template get<int>("key") } -> std::same_as<std::optional<int>>;
  { input.template get<std::string>("key") } -> std::same_as<std::optional<std::string>>;
};
}  // namespace VIPRA::Concepts
