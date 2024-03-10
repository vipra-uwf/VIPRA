#pragma once

#include <type_traits>

#include "vipra/concepts/parameters.hpp"

#define VIPRA_REGISTER_STEP                        \
  template <VIPRA::Concepts::ParamModule params_t> \
  void register_params(params_t& params)

#define VIPRA_REGISTER_PARAM(paramName) \
  params.register_param(_VIPRA_MODULE_TYPE_, _VIPRA_MODULE_NAME_, paramName);

#define VIPRA_CONFIG_STEP                          \
  template <VIPRA::Concepts::ParamModule params_t> \
  void config(params_t const& params, VIPRA::Random::Engine& engine)

#define VIPRA_GET_PARAM(paramName, output)                                \
  output = params.template get_param<std::remove_cv_t<decltype(output)>>( \
      _VIPRA_MODULE_TYPE_, _VIPRA_MODULE_NAME_, paramName, engine);

#define VIPRA_GET_ARRAY_PARAM(paramName, output)                                \
  output = params.template get_array_param<std::remove_cv_t<decltype(output)>>( \
      _VIPRA_MODULE_TYPE_, _VIPRA_MODULE_NAME_, paramName);
