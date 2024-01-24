#pragma once

#include <concepts>
#include <string>

#include "vipra/modules.hpp"
#include "vipra/types/parameter.hpp"

namespace VIPRA::Concepts {

template <typename params_t>
concept has_static_register_param = requires(const std::string& str) {
  {
    params_t::register_param(VIPRA::Modules::Type::PARAMETERS, str, ParameterType::REQUIRED)
    } -> std::same_as<void>;
};

template <typename params_t>
concept can_get_params = requires(const params_t params) {
  { params.template get_param<int>(VIPRA::Modules::Type::PARAMETERS, "") } -> std::same_as<int>;
};

template <typename params_t>
concept ParamModule =
    params_t::MODULE_TYPE ==
    VIPRA::Modules::Type::PARAMETERS&& has_static_register_param<params_t>&& can_get_params<params_t>;

struct DummyParams {
  // NOLINTBEGIN
  static constexpr VIPRA::Modules::Type MODULE_TYPE = VIPRA::Modules::Type::PARAMETERS;
  static void register_param(VIPRA::Modules::Type /*unused*/, const std::string& /*unused*/,
                             VIPRA::ParameterType /*unused*/) {}
  template <typename data_t>
  auto get_param(VIPRA::Modules::Type /*unused*/, const std::string& /*unused*/) const -> data_t {}
  // NOLINTEND
};

CHECK_MODULE(ParamModule, DummyParams)
}  // namespace VIPRA::Concepts