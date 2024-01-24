#pragma once

namespace VIPRA::Util {
template <typename func_t>
struct debug_do {
#ifndef NDEBUG
  explicit debug_do(func_t&& func) { func(); }
#else
  explicit debug_do(func_t&&) {}
#endif
};
}  // namespace VIPRA::Util