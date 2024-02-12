#pragma once

#include <type_traits>

namespace VIPRA::Concepts {
template <typename type_t>
concept is_void_type = !std::is_void_v<type_t>;

template <typename type_t>
concept is_not_void = !is_void_type<type_t>;

}  // namespace VIPRA::Concepts