#pragma once

#include "vipra/concepts/input.hpp"
#include "vipra/concepts/module.hpp"

#include "vipra/modules.hpp"

namespace VIPRA::Concepts {

template <typename input_t>
concept accepts_parameters = requires(input_t input) {
  { input.template get<VIPRA::Parameter<int>>("key") } -> std::same_as<std::optional<VIPRA::Parameter<int>>>;
  {
    input.template get<VIPRA::Parameter<std::string>>("key")
    } -> std::same_as<std::optional<VIPRA::Parameter<std::string>>>;
};

template <typename input_t>
concept parameter_qualified_input = is_type<input_t, Modules::Type::INPUT> && InputModule<input_t>;

class DummyParameterInput {
  // NOLINTBEGIN
 public:
  constexpr static VIPRA::Modules::Type MODULE_TYPE = VIPRA::Modules::Type::INPUT;

  explicit DummyParameterInput(std::string_view /*unused*/) {}

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

CHECK_MODULE(parameter_qualified_input, DummyParameterInput)

}  // namespace VIPRA::Concepts