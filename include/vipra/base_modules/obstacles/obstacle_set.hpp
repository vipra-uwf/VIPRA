#pragma once

#include "vipra/concepts/input.hpp"
#include "vipra/concepts/obstacle_set.hpp"

#include "vipra/modules.hpp"

#include "vipra/types/f3d.hpp"

namespace VIPRA {
class Obstacles {
  VIPRA_MODULE_TYPE(OBSTACLES)

 public:
  template <Concepts::InputModule input_t>
  explicit Obstacles(const input_t& input) {}

  void set_params(auto& params) {}

  auto get_map_dimensions() -> std::pair<VIPRA::f3d, VIPRA::f3d> {}
  auto collision(VIPRA::f3d) -> bool {}
  auto ray_hit(VIPRA::f3d, VIPRA::f3d) -> float {}
  auto nearest_obstacle(VIPRA::f3d) -> VIPRA::f3d {}
  auto nearest_obstacle_in_direction(VIPRA::f3d, VIPRA::f3d) -> VIPRA::f3d {}
  auto get_object_types() -> const std::vector<std::string>& {}
  auto get_objects(const std::string&) -> const VIPRA::f3dVec& {}
};
}  // namespace VIPRA

CHECK_MODULE(ObstacleModule, VIPRA::Obstacles);