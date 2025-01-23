
#pragma once

#include <cstddef>

#include "vipra/geometry/f3d.hpp"

namespace VIPRA {
struct Sound {
  size_t     stimulus_id;
  VIPRA::f3d position;
};
}  // namespace VIPRA