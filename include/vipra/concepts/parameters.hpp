#pragma once

#include <concepts>
#include <string>

#include "vipra/macros/module.hpp"
#include "vipra/modules.hpp"

#include "vipra/types/parameter.hpp"

namespace VIPRA::Concepts {

template <typename params_t>
concept can_register_param = requires(params_t params, VIPRA::Modules::Type module,
                                      const std::string& moduleName, const std::string& paramName,
                                      ParameterType param) {
  { params.register_param(module, moduleName, paramName, param) } -> std::same_as<void>;
};

template <typename params_t>
concept can_get_params = requires(params_t params, VIPRA::Modules::Type module, const std::string& moduleName,
                                  const std::string& paramName) {
  { params.template get_param<int>(module, moduleName, paramName) } -> std::same_as<int>;
};

template <typename params_t>
concept ParamModule =
    params_t::_VIPRA_MODULE_TYPE_ ==
    VIPRA::Modules::Type::PARAMETERS&& can_register_param<params_t>&& can_get_params<params_t>;

struct DummyParams {
  // NOLINTBEGIN
  static constexpr VIPRA::Modules::Type _VIPRA_MODULE_TYPE_ = VIPRA::Modules::Type::PARAMETERS;

  void register_param(VIPRA::Modules::Type, const std::string&, const std::string&, ParameterType) {}

  template <typename data_t>
  auto get_param(VIPRA::Modules::Type, const std::string&, const std::string&) const -> data_t {}
  // NOLINTEND
};

CHECK_MODULE(ParamModule, DummyParams)
}  // namespace VIPRA::Concepts