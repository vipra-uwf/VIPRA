#pragma once

#include <tuple>
#include <type_traits>
#include <utility>
#include "vipra/util/get_nth_value.hpp"

namespace VIPRA::Util {
// NOLINTBEGIN(readability-identifier-naming) utility type
template <typename... type_ts>
struct all_of_type {};

template <typename type_t>
struct all_of_type<type_t> {
  static constexpr bool value = true;
};

template <typename check_type_t, typename head_t, typename... tail_ts>
struct all_of_type<check_type_t, head_t, tail_ts...> {
  static constexpr bool value =
      std::is_same_v<check_type_t, head_t> && all_of_type<head_t, tail_ts...>::value;
};

template <typename check_type_t, typename... type_ts>
struct all_of_type<check_type_t, std::tuple<type_ts...>> {
  static constexpr bool value = std::is_same_v<check_type_t, std::tuple_element<0, std::tuple<type_ts...>>> &&
                                all_of_type<std::tuple_element<0, std::tuple<type_ts...>>, type_ts...>::value;
};

/**
 * @brief true if all of the tuple types are the same, false if not
 * 
 * @tparam type_ts 
 */
template <typename... type_ts>
inline constexpr bool all_of_type_v = all_of_type<type_ts...>::value;
// NOLINTEND(readability-identifier-naming)
}  // namespace VIPRA::Util