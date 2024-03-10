#pragma once

#include "vipra/concepts/input.hpp"
#include "vipra/concepts/module.hpp"

#include "vipra/modules.hpp"

namespace VIPRA::Concepts {

/**
 * @brief Checks that a type can supply parameters for a given key
 * 
 * @tparam input_t 
 */
template <typename input_t>
concept accepts_parameters = requires(input_t input) {
  { input.template get<VIPRA::Parameter<int>>("key") } -> std::same_as<std::optional<VIPRA::Parameter<int>>>;
  {
    input.template get<VIPRA::Parameter<std::string>>("key")
    } -> std::same_as<std::optional<VIPRA::Parameter<std::string>>>;
};

/**
 * @brief Checks that a type is a parameter qualified input module
 * 
 * @tparam input_t 
 */
template <typename input_t>
concept parameter_qualified_input = accepts_parameters<input_t> && InputModule<input_t>;

/**
 * @brief Dummy parameter input for use in other concepts
 * 
 */
class DummyParameterInput {
  // NOLINTBEGIN
 public:
  constexpr static VIPRA::Modules::Type _VIPRA_MODULE_TYPE_ = VIPRA::Modules::Type::INPUT;

  template <typename params_t>
  void register_params(params_t&) {}

  void config(auto const&) {}

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

CHECK_MODULE(parameter_qualified_input, DummyParameterInput)

}  // namespace VIPRA::Concepts