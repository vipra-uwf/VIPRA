#pragma once

#include <type_traits>

namespace VIPRA::Concepts {
template <typename type_t, typename = void>
struct has_type : std::false_type {};

template <typename type_t>
struct has_type<type_t, std::void_t<decltype(type_t::MODULE_TYPE)>> : std::true_type {};
}  // namespace VIPRA::Concepts