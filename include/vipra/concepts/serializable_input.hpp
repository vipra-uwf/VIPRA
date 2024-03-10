#pragma once

#include <concepts>
#include <string>
#include <type_traits>
#include "vipra/concepts/input.hpp"
#include "vipra/concepts/parameters.hpp"

namespace VIPRA::Concepts {

/**
 * @brief Checks that a type can be serialized and deserialized
 * 
 * @tparam input_t 
 */
template <typename input_t>
concept serializable = requires(input_t input) {
  { input.serialize() } -> std::same_as<std::string>;
  { input.deserialize(std::declval<std::string>()) } -> std::same_as<void>;
};

/**
 * @brief Checks that a type is a serializable input module
 * 
 * @tparam input_t 
 */
template <typename input_t>
concept serializable_input = InputModule<input_t> && serializable<input_t>;
}  // namespace VIPRA::Concepts