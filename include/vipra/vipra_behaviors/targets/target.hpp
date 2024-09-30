#pragma once

#include <functional>

#include "definitions/sim_pack.hpp"
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

/**
 * @brief Function for selecting targets
 * 
 */
using TargetFunc = std::function<Target(Simpack, Self)>;
}  // namespace VIPRA::Behaviors
