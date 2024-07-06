#pragma once

#include <map>
namespace VIPRA::Util {
// NOLINTBEGIN

template <class data_t>
struct is_map {
  static constexpr bool value = false;
};

template <class key_t, class value_t>
struct is_map<std::map<key_t, value_t>> {
  static constexpr bool value = true;
};

template <typename data_t>
static constexpr bool is_map_v = is_map<data_t>::value;

// NOLINTEND
}  // namespace VIPRA::Util