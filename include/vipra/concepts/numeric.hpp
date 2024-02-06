#pragma once

#include <concepts>
#include <type_traits>

namespace VIPRA::Concepts {
template <class type_t>
concept Numeric = std::is_arithmetic_v<std::remove_reference_t<type_t>>;
}