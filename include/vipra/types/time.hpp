#pragma once

#include <cstdint>
#include <utility>

#include "vipra/types/float.hpp"

namespace VIPRA {
using timestep = std::uint64_t;

using delta_t = VIPRA::f_pnt;

using time_s = VIPRA::f_pnt;
using time_ms = VIPRA::f_pnt;

using time_range_s = std::pair<time_s, time_s>;
using time_range_ms = std::pair<time_ms, time_ms>;
}  // namespace VIPRA