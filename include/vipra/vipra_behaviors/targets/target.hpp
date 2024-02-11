#pragma once

#include <functional>

#include "vipra/geometry/f3d.hpp"
#include "vipra/types/idx.hpp"

namespace VIPRA::Behaviors {
enum class TargetType {
  INVALID,
  PEDESTRIAN,
  LOCATION,
  EVENT,
};

/**
 * @brief Holds information about what to target
 * 
 */
struct Target {
  TargetType type;
  VIPRA::idx targetIdx;
};

/**
 * @brief Struct for explicitly targeting the self
 * 
 */
struct Self {
  Target target;
};

template <typename simpack_t>
/**
 * @brief Function for selecting targets
 * 
 */
using TargetFunc = std::function<Target(simpack_t, Self)>;
}  // namespace VIPRA::Behaviors
