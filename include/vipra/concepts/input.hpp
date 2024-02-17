#pragma once

#include <concepts>
#include <map>
#include <optional>
#include <string_view>
#include <type_traits>
#include <vector>

#include "vipra/concepts/module.hpp"
#include "vipra/modules.hpp"

#include "vipra/geometry/f3d.hpp"
#include "vipra/types/parameter.hpp"

// TODO(rolland): remember to add in documentation that inputs should hold off on loading until their load() method is called

namespace VIPRA::Concepts {

// TODO(rolland): clean this up and make it more generic/strict
template <typename input_t>
concept can_get_values = requires(input_t input, std::string_view key, std::string_view key2) {
  { input.template get<int>("key") } -> std::same_as<std::optional<int>>;
  { input.template get<std::string>("key") } -> std::same_as<std::optional<std::string>>;
  { input.template get<std::vector<int>>("key") } -> std::same_as<std::optional<std::vector<int>>>;
};

// TODO(rolland): this is a sort of work around for making sure that inputs aren't loaded on every node for a parameter sweep
//                  - input should only be loaded when this function is called
//                  - but nothing is stopping implementers from loading parameters in the constructor/elsewhere
//                  - not sure if there is a good way to only load parameters on one node while still allowing any input to be used for parameters
template <typename input_t>
concept delayed_load = requires(input_t input) {
  { input.load() } -> std::same_as<void>;
};

template <typename input_t>
concept InputModule =
    is_type<input_t, VIPRA::Modules::Type::INPUT> && can_get_values<input_t> && delayed_load<input_t>;

class DummyInput {
  // NOLINTBEGIN
 public:
  constexpr static VIPRA::Modules::Type _VIPRA_MODULE_TYPE_ = VIPRA::Modules::Type::INPUT;

  void load() {}

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
