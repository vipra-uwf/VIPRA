#pragma once

#include "vipra/macros/collision_avoidance.hpp"

#include "vipra/modules/goals.hpp"
#include "vipra/modules/map.hpp"
#include "vipra/modules/pedestrians.hpp"

namespace VIPRA::Modules {
class CollisionAvoidance {
 public:
  virtual VIPRA_COLLISION_UPDATE = 0;
  virtual VIPRA_COLLISION_INIT_STEP = 0;

  CollisionAvoidance() = default;
  CollisionAvoidance(const CollisionAvoidance&) = default;
  CollisionAvoidance(CollisionAvoidance&&) = default;
  auto operator=(const CollisionAvoidance&) -> CollisionAvoidance& = default;
  auto operator=(CollisionAvoidance&&) -> CollisionAvoidance& = default;
  virtual ~CollisionAvoidance() = default;
};
}  // namespace VIPRA::Modules