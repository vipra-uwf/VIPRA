#pragma once

#include <type_traits>

namespace VIPRA {
template <typename type_t, typename = int>
// NOLINTNEXTLINE
struct has_tag : std::false_type {};

template <typename type_t>
struct has_tag<type_t, decltype((void)type_t::tag, 0)> : std::true_type {};
}  // namespace VIPRA