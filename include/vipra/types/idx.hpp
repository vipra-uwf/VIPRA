#pragma once

#include <cstdint>
#include <limits>
#include <vector>

namespace VIPRA {
using idx = std::uint64_t;

using idxVec = std::vector<idx>;

static constexpr VIPRA::idx INVALID_IDX = std::numeric_limits<VIPRA::idx>::max();
}  // namespace VIPRA