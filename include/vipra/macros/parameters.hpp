#pragma once

#include <tuple>

#define VIPRA_REGISTER_PARAMS(params...) \
  [[nodiscard]] auto module_params() { return std::make_tuple(params); }

#define VIPRA_REGISTER_BASE_PARAMS(params...) \
  [[nodiscard]] auto base_module_params() { return std::make_tuple(params); }

#define VIPRA_PARAM(name, output) \
  std::pair<char const*, std::remove_cvref_t<decltype(output)>&> { name, output }
