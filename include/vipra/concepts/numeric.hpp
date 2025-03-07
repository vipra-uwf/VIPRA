#pragma once

#include <concepts>
#include <type_traits>

namespace VIPRA::Concepts {
/**
 * @brief Checks that a type is a numeric type
 * 
 * @tparam type_t 
 */
template <class type_t>
concept Numeric =
    std::is_arithmetic_v<std::remove_reference_t<type_t>> &&
    ! std::is_same_v<type_t, bool> && ! std::is_same_v<type_t, char> &&
    ! std::is_same_v<type_t, const bool> && ! std::is_same_v<type_t, const char>;
}  // namespace VIPRA::Concepts