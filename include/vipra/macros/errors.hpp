#pragma once

#include "vipra/logging/logging.hpp"

#define VIPRA_MODULE_ERROR(message, ...)                                \
  {                                                                     \
    VIPRA::Log::error("{} Module Error: {}\nAt: {}:{} ", module_name(), \
                      fmt::format(message __VA_OPT__(, ) __VA_ARGS__),  \
                      __FILE__, __LINE__);                              \
    throw std::runtime_error(                                           \
        fmt::format("{} Module Error: {}", module_name(),               \
                    fmt::format(message __VA_OPT__(, ) __VA_ARGS__)));  \
  }

#define VIPRA_BASE_MODULE_ERROR(type, message, ...)                    \
  {                                                                    \
    VIPRA::Log::error("{} Module Error: {}\nAt: {}:{}", type,          \
                      fmt::format(message __VA_OPT__(, ) __VA_ARGS__), \
                      __FILE__, __LINE__);                             \
    throw std::runtime_error(                                          \
        fmt::format("{} Module Error: {}", type,                       \
                    fmt::format(message __VA_OPT__(, ) __VA_ARGS__))); \
  }
