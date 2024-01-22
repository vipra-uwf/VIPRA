#pragma once

#include <concepts>
#include <string>

#include "vipra/modules.hpp"
#include "vipra/types/parameter.hpp"

namespace VIPRA::Concepts {
template <typename params_t>
concept ParamModule = requires(params_t params, const std::string& str) {
  params_t::MODULE_TYPE == VIPRA::Modules::Type::PARAMETERS;
  { params.set_param(VIPRA::Modules::Type::PARAMETERS, str, Parameter{}) } -> std::same_as<void>;
  { params.template get_param<int>(VIPRA::Modules::Type::PARAMETERS, "") } -> std::same_as<int>;
};

struct DummyParams {
  static constexpr VIPRA::Modules::Type MODULE_TYPE = VIPRA::Modules::Type::PARAMETERS;
  void set_param(VIPRA::Modules::Type /*unused*/, const std::string& /*unused*/,
                 VIPRA::Parameter /*unused*/) {}
  template <typename data_t>
  auto get_param(VIPRA::Modules::Type /*unused*/, const std::string& /*unused*/) -> data_t {}
};

static_assert(Concepts::ParamModule<DummyParams>, "DummyParams does not satisfy ParamModule");
}  // namespace VIPRA::Concepts