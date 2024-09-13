#pragma once

#define FORWARD_REGISTER_PARAMS \
  [[nodiscard]] auto parameters() { return self().module_params(); }

#define REGISTER_BASE_PARAMS(params...) \
  [[nodiscard]] auto parameters() { return std::tuple_cat(std::make_tuple(params), self().module_params()); }
