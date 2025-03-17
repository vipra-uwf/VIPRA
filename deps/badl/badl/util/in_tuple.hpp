#pragma once

#include <tuple>
#include <type_traits>

namespace BADL {
template <typename T, typename Tuple>
struct is_in_tuple {
  static constexpr bool value = false;
};

template <typename T, typename... Types>
struct is_in_tuple<T, std::tuple<Types...>> {
  static constexpr bool value = std::disjunction_v<std::is_same<T, Types>...>;
};
}  // namespace BADL