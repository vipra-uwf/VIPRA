#pragma once

#include <cstddef>

namespace VIPRA::Util {
template <std::size_t index_t, template <typename> typename check_t, typename... types_t>
struct FindIndex;

template <std::size_t index_t, template <typename> typename check_t, typename type_t,
          typename... types_t>
struct FindIndex<index_t, check_t, type_t, types_t...> {
  // NOLINTNEXTLINE(readability-identifier-naming) lowercase is a regular naming convetion here
  static constexpr std::size_t value =
      check_t<type_t>::value ? index_t
                             : FindIndex<index_t + 1, check_t, types_t...>::value;
};

/**
 * @brief Finds the index of a type in a tuple
 * 
 * @tparam index_t 
 * @tparam check_t 
 */
template <std::size_t index_t, template <typename> typename check_t>
struct FindIndex<index_t, check_t> {
  // NOLINTNEXTLINE(readability-identifier-naming) lowercase is a regular naming convetion here
  static constexpr std::size_t value = -1;
};
}  // namespace VIPRA::Util