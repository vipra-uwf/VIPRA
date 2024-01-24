#pragma once

#include <concepts>

namespace VIPRA::Concepts {
template <class type_t>
concept Numeric = std::is_arithmetic_v<type_t>;
}