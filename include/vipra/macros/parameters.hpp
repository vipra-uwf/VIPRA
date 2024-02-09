#pragma once

#include <type_traits>

#include "vipra/concepts/parameters.hpp"

#define VIPRA_REGISTER_STEP                        \
  template <VIPRA::Concepts::ParamModule params_t> \
  void register_params(params_t& params)

#define VIPRA_REGISTER_PARAM(paramName, param) \
  params.register_param(_VIPRA_MODULE_TYPE_, _VIPRA_MODULE_NAME_, paramName, VIPRA::ParameterType::param);

#define VIPRA_CONFIG_STEP                          \
  template <VIPRA::Concepts::ParamModule params_t> \
  void config(const params_t& params)

#define VIPRA_GET_PARAM(paramName, output)                                                    \
  if (!params.has_required_param(_VIPRA_MODULE_TYPE_, _VIPRA_MODULE_NAME_, paramName)) {      \
    throw std::runtime_error("Required Parameter: " paramName " For Module: " +               \
                             to_string(_VIPRA_MODULE_TYPE_) + " Not Provide in Input");       \
  }                                                                                           \
  output = params.template get_param<std::remove_cv_t<decltype(output)>>(_VIPRA_MODULE_TYPE_, \
                                                                         _VIPRA_MODULE_NAME_, paramName);
