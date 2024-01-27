#pragma once

#define VIPRA_REGISTER_STEP                        \
  template <VIPRA::Concepts::ParamModule params_t> \
  void register_params(params_t& params)

#define VIPRA_REGISTER_PARAM(module, paramName, param) \
  params.register_param(VIPRA::Modules::Type::module, _VIPRA_MODULE_NAME_, paramName, param)

#define VIPRA_CONFIG_STEP                          \
  template <VIPRA::Concepts::ParamModule params_t> \
  void config(const params_t& params)