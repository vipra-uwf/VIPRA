#pragma once

#include <cstddef>
#include <vector>
#include "vipra/types/tags.hpp"
namespace VIPRA {
struct Parameter {
  enum class RandomType { NONE, RANGE, DISCRETE };
};
}  // namespace VIPRA