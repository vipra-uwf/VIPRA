#ifndef VIPRA_BEHAVIORS_TARGET_HPP
#define VIPRA_BEHAVIORS_TARGET_HPP

#include <functional>

#include <definitions/behavior_context.hpp>
#include <definitions/sim_pack.hpp>
#include <goals/goals.hpp>
#include <obstacle_set/obstacle_set.hpp>
#include <pedestrian_set/pedestrian_set.hpp>
#include <random/random.hpp>
#include "vipra/geometry/f3d.hpp"

namespace BHVR {
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
}  // namespace BHVR

#endif