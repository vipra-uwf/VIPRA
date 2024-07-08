#pragma once

namespace VIPRA::Debug {

/**
 * @brief Utility function that executes a function in debug mode only
 * 
 */
#ifdef VIPRA_DEBUG_MODE
template <typename func_t>
void debug_do(func_t&& func) {
  func();
}
#else
template <typename func_t>
void debug_do(func_t&& /*unused*/) {}
#endif
}  // namespace VIPRA::Debug