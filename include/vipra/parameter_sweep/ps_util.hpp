#pragma once

#include <cstddef>

namespace VIPRA {
[[nodiscard]] inline auto sim_count(size_t rank, size_t size, size_t totSimCount) -> size_t {
  size_t localCount = totSimCount / size;
  if ( rank < totSimCount % size ) {
    ++localCount;
  }

  return localCount;
}

[[nodiscard]] inline auto start_sim_id(size_t rank, size_t size, size_t totSimCount) -> size_t {
  size_t idx = rank * sim_count(rank, size, totSimCount);
  if ( rank >= totSimCount % size ) {
    idx += totSimCount % size;
  }

  return idx;
}
}  // namespace VIPRA