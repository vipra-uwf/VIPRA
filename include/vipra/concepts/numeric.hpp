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
concept Numeric = std::is_arithmetic_v<std::remove_reference_t<type_t>>;
}  // namespace VIPRA::Concepts