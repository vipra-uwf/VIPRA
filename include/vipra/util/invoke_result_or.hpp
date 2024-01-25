#pragma once

#include <type_traits>

#include "vipra/types/util/result_or_void.hpp"

namespace VIPRA::Util {
template <typename result_t, typename func_t, typename... arg_ts>
// NOLINTNEXTLINE(readability-identifier-naming) utility struct
struct invoke_result_or {
  using type =
      std::conditional<std::is_same_v<func_t, VOID>, result_t, std::invoke_result_t<func_t, arg_ts...>>::type;
};

template <typename result_t, typename func_t, typename... arg_ts>
using invoke_result_or_t = typename invoke_result_or<result_t, func_t, arg_ts...>::type;
}  // namespace VIPRA::Util