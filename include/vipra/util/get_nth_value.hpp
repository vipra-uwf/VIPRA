#pragma once

#include <tuple>

namespace VIPRA::Util {
template <std::size_t n, typename... values>
constexpr auto get_nth_value(values&&... vals) -> decltype(auto) {
  return std::get<n>(std::forward_as_tuple(std::forward<values>(vals)...));
}
}  // namespace VIPRA::Util