#pragma once

#define VIPRA_REGISTER_STEP                        \
  template <VIPRA::Concepts::ParamModule params_t> \
  void register_params(params_t& params)

#define VIPRA_REGISTER_PARAM(paramName, param) \
  params.register_param(_VIPRA_MODULE_TYPE_, _VIPRA_MODULE_NAME_, paramName, VIPRA::ParameterType::param);

#define VIPRA_CONFIG_STEP                          \
  template <VIPRA::Concepts::ParamModule params_t> \
  void config(const params_t& params)

#define VIPRA_GET_PARAM(paramName, type) \
  params.template get_param<type>(_VIPRA_MODULE_TYPE_, _VIPRA_MODULE_NAME_, paramName);
