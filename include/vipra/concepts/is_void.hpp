#pragma once

#include <type_traits>
namespace VIPRA::Concepts {
template <typename data_t>
concept is_void_type = std::is_void_v<data_t>;

template <typename data_t>
concept is_not_void_type = ! is_void_type<data_t>;
}  // namespace VIPRA::Concepts