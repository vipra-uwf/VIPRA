#pragma once

#include <cstdint>
#include <limits>

namespace VIPRA {
using idx = std::uint64_t;

static constexpr VIPRA::idx INVALID_IDX = std::numeric_limits<VIPRA::idx>::max();
}  // namespace VIPRA