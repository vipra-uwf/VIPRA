#pragma once

#include <type_traits>

#define VIPRA_REGISTER_STEP [[nodiscard]] auto parameters()

#define VIPRA_REGISTER_PARAM(name, output) std::pair<std::string, decltype(output)&>{name, output};

#define VIPRA_CONFIG_STEP           \
  template <typename paramreader_t> \
  void config(paramreader_t& paramIn, VIPRA::Random::Engine& engine)

#define VIPRA_GET_PARAM(paramName, output)                                \
  output = params.template get_param<std::remove_cv_t<decltype(output)>>( \
      _VIPRA_MODULE_TYPE_, _VIPRA_MODULE_NAME_, paramName, engine);

#define VIPRA_GET_ARRAY_PARAM(paramName, output)                                \
  output = params.template get_array_param<std::remove_cv_t<decltype(output)>>( \
      _VIPRA_MODULE_TYPE_, _VIPRA_MODULE_NAME_, paramName);
