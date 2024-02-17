#pragma once

#include <concepts>
#include <optional>
#include <string>
#include <type_traits>
#include <vector>

#include "vipra/macros/module.hpp"
#include "vipra/modules.hpp"

#include "vipra/random/random.hpp"
#include "vipra/types/parameter.hpp"

// TODO(rolland): add remove_cvref_t to all the concepts

namespace VIPRA::Concepts {

template <typename params_t>
concept can_register_param = requires(params_t params, VIPRA::Modules::Type module,
                                      std::string const& moduleName, std::string const& paramName) {
  { params.register_param(module, moduleName, paramName) } -> std::same_as<void>;
};

template <typename params_t>
concept can_get_params = requires(params_t params, VIPRA::Modules::Type module, std::string const& moduleName,
                                  std::string const& paramName, VIPRA::Random::Engine& engine) {
  { params.template get_param<int>(module, moduleName, paramName, engine) } -> std::same_as<int>;
  {
    params.template get_array_param<std::vector<int>>(module, moduleName, paramName)
    } -> std::same_as<std::vector<int>>;
};

template <typename params_t>
concept can_get_input = requires(params_t params, VIPRA::Modules::Type module, std::string const& moduleName,
                                 std::string const& paramName) {
  {params.get_input()};
};

template <typename params_t>
concept ParamModule =
    std::remove_cvref_t<params_t>::_VIPRA_MODULE_TYPE_ ==
    VIPRA::Modules::Type::PARAMETERS&& can_register_param<std::remove_cvref_t<params_t>>&&
        can_get_params<std::remove_cvref_t<params_t>>&& can_get_input<std::remove_cvref_t<params_t>>;

struct DummyParams {
  // NOLINTBEGIN
  static constexpr VIPRA::Modules::Type _VIPRA_MODULE_TYPE_ = VIPRA::Modules::Type::PARAMETERS;

  void register_param(VIPRA::Modules::Type, std::string const&, std::string const&) {}

  auto get_input() -> std::string { return ""; }

  template <typename data_t>
  auto get_param(VIPRA::Modules::Type, std::string const&, std::string const&, VIPRA::Random::Engine&) const
      -> data_t {}

  template <typename data_t>
  auto get_array_param(VIPRA::Modules::Type, std::string const&, std::string const&) const -> data_t {}
  // NOLINTEND
};

CHECK_MODULE(ParamModule, DummyParams)
}  // namespace VIPRA::Concepts