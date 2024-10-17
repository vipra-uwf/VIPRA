#pragma once

#include <cstdint>

namespace VIPRA::Behaviors {
/**
 * @brief Unique identifier for a pedestrian/environment state
 * 
 */
enum stateUID : uint64_t;

constexpr inline auto operator++(stateUID& val) noexcept -> stateUID&
{
  return val = static_cast<stateUID>(static_cast<uint64_t>(val) + 1);
}

constexpr inline auto operator++(stateUID& val, int) noexcept -> stateUID
{
  stateUID tmp{val};
  ++val;
  return tmp;
}

}  // namespace VIPRA::Behaviors
