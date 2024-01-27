#pragma once

namespace VIPRA::Util {
template <typename func_t>
/**
 * @brief Utility Class that executes a function in debug mode only
 * 
 */
// NOLINTNEXTLINE(readability-identifier-naming) utilty class
struct debug_do {
#ifndef NDEBUG
  explicit debug_do(func_t&& func) { func(); }
#else
  explicit debug_do(func_t&& /*unused*/) {}
#endif
};
}  // namespace VIPRA::Util