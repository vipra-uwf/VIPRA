#pragma once

#include <type_traits>

#include "vipra/types/util/result_or_void.hpp"

namespace VIPRA::Util {
/**
   * @brief Utility class for getting the result of a function or a default type if the function returns void
   * 
   * @tparam result_t 
   * @tparam func_t 
   * @tparam arg_ts 
   */
template <typename result_t, typename func_t, typename... arg_ts>
// NOLINTNEXTLINE(readability-identifier-naming) utility struct
struct invoke_result_or {
  using type = typename std::conditional<std::is_same_v<func_t, VOID>, result_t,
                                         std::invoke_result_t<func_t, arg_ts...>>::type;
};

/**
 * @brief Utility alias for getting the result of a function or a default type if the function returns void
 * 
 * @tparam result_t 
 * @tparam func_t 
 * @tparam arg_ts 
 */
template <typename result_t, typename func_t, typename... arg_ts>
using invoke_result_or_t = typename invoke_result_or<result_t, func_t, arg_ts...>::type;
}  // namespace VIPRA::Util