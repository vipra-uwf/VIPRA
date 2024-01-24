#pragma once

#include <tuple>

namespace VIPRA {

/**
 * @brief Placeholder Type for void
 * 
 */
struct VOID {};

template <typename>
struct result_or_VOID;

template <typename type_t>
struct result_or_VOID {
  using type = std::conditional_t<std::is_void_v<type_t>, VOID, type_t>;
};

template <typename data_t>
using result_or_VOID_t = typename result_or_VOID<data_t>::type;

// template <typename>
// struct result_or_VOID_tuple;

// /**
//  * @brief result_or_void base case, returns void
//  *
//  * @tparam
//  */
// template <>
// struct result_or_VOID_tuple<std::tuple<>> {
//   using type = VOID;
// };

// /**
//  * @brief Replaces a tuple with a tuple of the same size, but with voids replaced with VOID
//  *
//  * @tparam head_t
//  * @tparam tail_t
//  */
// template <typename head_t, typename... tail_t>
// struct result_or_VOID_tuple<std::tuple<head_t, tail_t...>> {
//   using type = std::tuple<std::conditional_t<std::is_void_v<head_t>, VOID, head_t>,
//                           typename result_or_VOID_tuple<tail_t...>::type>;
// };
}  // namespace VIPRA