#pragma once

#include <type_traits>

namespace VIPRA::Util {
template <typename type_t, template <typename...> class ref_t>
// NOLINTNEXTLINE
struct is_specialization : std::false_type {};

template <template <typename...> class ref_t, typename... arg_ts>
struct is_specialization<ref_t<arg_ts...>, ref_t> : std::true_type {};

template <typename>
struct get_specialization_internal;

/**
 * @brief Utility struct that gets the specialization of a templated type
 * 
 * @tparam template_t 
 * @tparam inner_t 
 */
template <template <typename...> typename template_t, typename inner_t>
struct get_specialization_internal<template_t<inner_t>> {
  using type = inner_t;
};

}  // namespace VIPRA::Util