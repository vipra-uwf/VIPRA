#pragma once

#include <tuple>
#include "vipra/util/get_nth_value.hpp"
namespace VIPRA::Util {

/**
 * @brief Utility function that returns the tail of a tuple
 * 
 * @tparam head 
 * @tparam tail 
 * @param tuple 
 * @return std::tuple<tail...> 
 */
template <typename head, typename... tail>
constexpr auto tuple_tail(const std::tuple<head, tail...>& tuple) -> std::tuple<tail...> {
  return std::apply([](auto&&, const auto&... args) { return std::tie(args...); }, tuple);
}
}  // namespace VIPRA::Util