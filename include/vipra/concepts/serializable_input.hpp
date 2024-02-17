#pragma once

#include <concepts>
#include <string>
#include <type_traits>
#include "vipra/concepts/input.hpp"
#include "vipra/concepts/parameters.hpp"

namespace VIPRA::Concepts {

template <typename input_t>
concept serializable = requires(input_t input) {
  { input.serialize() } -> std::same_as<std::string>;
  { input.deserialize(std::declval<std::string>()) } -> std::same_as<void>;
};

template <typename input_t>
concept serializable_input = InputModule<input_t> && serializable<input_t>;
}  // namespace VIPRA::Concepts