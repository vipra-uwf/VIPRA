#pragma once

#include <concepts>
#include <string_view>
#include <type_traits>

namespace VIPRA::Concepts {
template <typename type_t>
concept StringView =
    std::is_same_v<type_t, std::string_view> || std::is_convertible_v<type_t, std::string_view>;
}