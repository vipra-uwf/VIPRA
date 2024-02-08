#pragma once

#include <cstddef>
#include "vipra/types/tags.hpp"
namespace VIPRA {
enum class ParameterType { REQUIRED, OPTIONAL };

template <typename data_t>
struct Parameter {
  enum class RandomType { NONE, RANGE, DISCRETE };

  // TODO(rolland): this should have min/max, choices etc. not a single value
  RandomType randomType = RandomType::NONE;
  data_t     value;

  // NOLINTNEXTLINE
  static constexpr Tag tag = Tag::PARAMETER;
  using internal_t = data_t;
};
}  // namespace VIPRA