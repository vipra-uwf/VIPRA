#pragma once

#include <concepts>
#include <map>
#include <optional>
#include <string_view>
#include <type_traits>
#include <vector>

#include "vipra/concepts/module.hpp"
#include "vipra/modules.hpp"

#include "vipra/types/f3d.hpp"
#include "vipra/types/parameter.hpp"

namespace VIPRA::Concepts {

// TODO(rolland): clean this up and make it more generic/strict
template <typename input_t>
concept can_get_values = requires(input_t input, std::string_view key, std::string_view key2) {
  { input.template get_vector<int>(key) } -> std::same_as<std::optional<std::vector<int>>>;
  { input.template get_vector<int>(key, key2) } -> std::same_as<std::optional<std::vector<int>>>;
  { input.template get_vector<std::string>(key) } -> std::same_as<std::optional<std::vector<std::string>>>;
  {
    input.template get_vector<std::string>(key, key2)
    } -> std::same_as<std::optional<std::vector<std::string>>>;

  { input.template get_vector<VIPRA::f3d>(key) } -> std::same_as<std::optional<std::vector<VIPRA::f3d>>>;
  { input.template get<int>("key") } -> std::same_as<std::optional<int>>;
  { input.template get<std::string>("key") } -> std::same_as<std::optional<std::string>>;
};

template <typename input_t>
concept InputModule = is_type<input_t, Modules::Type::INPUT> &&
    std::constructible_from<input_t, std::string_view> && can_get_values<input_t>;

class DummyInput {
  // NOLINTBEGIN
  VIPRA_MODULE_TYPE(INPUT)
 public:
  explicit DummyInput(std::string_view /*unused*/) {}

  template <typename data_t>
  auto get(std::string_view /*unused*/, std::string_view /*unused*/) const -> std::optional<data_t> {
    return std::nullopt;
  }

  template <typename data_t>
  auto get(std::string_view /*unused*/) const -> std::optional<data_t> {
    return std::nullopt;
  }

  template <typename data_t>
  auto get_vector(std::string_view /*unused*/, std::string_view /*unused*/) const
      -> std::optional<std::vector<data_t>> {
    return std::nullopt;
  }

  template <typename data_t>
  auto get_vector(std::string_view /*unused*/) const -> std::optional<std::vector<data_t>> {
    return std::nullopt;
  }
  // NOLINTEND
};

CHECK_MODULE(InputModule, DummyInput);

}  // namespace VIPRA::Concepts
