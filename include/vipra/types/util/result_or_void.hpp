#pragma once

#include <tuple>

namespace VIPRA {
/**
 * @brief Placeholder Type for void
 * 
 */
struct VOID {
  template <typename... arg_ts>
  inline void operator()(arg_ts... /*unused*/) const noexcept
  {
  }
};
}  // namespace VIPRA

namespace VIPRA::Util {

template <typename>
struct result_or_VOID;

template <typename type_t>
struct result_or_VOID {
  using type = std::conditional_t<std::is_void_v<type_t>, VOID, type_t>;
};

template <typename data_t>
using result_or_VOID_t = typename result_or_VOID<data_t>::type;
}  // namespace VIPRA::Util