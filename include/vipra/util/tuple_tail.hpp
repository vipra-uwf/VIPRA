#pragma once

#include <tuple>
#include "vipra/util/get_nth_value.hpp"
namespace VIPRA::Util {
// make function that returns a tuple of all but the first element
template <typename head, typename... tail>
constexpr auto tuple_tail(const std::tuple<head, tail...>& tuple) -> std::tuple<tail...> {
  return std::apply([](auto&&, const auto&... args) { return std::tie(args...); }, tuple);
}
}  // namespace VIPRA::Util