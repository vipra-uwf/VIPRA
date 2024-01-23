#pragma once

#include "vipra/concepts/input.hpp"
#include "vipra/concepts/obstacle_set.hpp"

#include "vipra/concepts/parameters.hpp"
#include "vipra/modules.hpp"

#include "vipra/types/f3d.hpp"
#include "vipra/types/float.hpp"

// TODO(rolland): implement quadtree for storing obstacles

namespace VIPRA::Obstacles {
class QuadTree {
  VIPRA_MODULE_TYPE(OBSTACLES);

 public:
  template <Concepts::InputModule input_t>
  explicit QuadTree(const input_t& input) {}

  template <typename params_t>
  static void register_params() {
    // TODO: register parameters
  }

  void setup(auto& params) {
    // TODO: load parameters
  }

  // NOLINTBEGIN rolland: Dummy Object that cannot be used, we don't care about errors here
  auto get_map_dimensions() -> std::pair<VIPRA::f3d, VIPRA::f3d> { return {VIPRA::f3d{}, VIPRA::f3d{}}; }
  auto collision(VIPRA::f3d) -> bool { return false; }
  auto ray_hit(VIPRA::f3d, VIPRA::f3d) -> VIPRA::f_pnt { return 1.0F; }
  auto nearest_obstacle(VIPRA::f3d) -> VIPRA::f3d { return VIPRA::f3d{}; }
  auto nearest_obstacle_in_direction(VIPRA::f3d, VIPRA::f3d) -> VIPRA::f3d { return VIPRA::f3d{}; }
  auto get_object_types() -> const std::vector<std::string>& { return _dummy2; }
  auto get_objects(const std::string&) -> const VIPRA::f3dVec& { return _dummy; }
  // NOLINTEND

 private:
  // TODO(rolland): remove this dummy object, implement obstacle set
  VIPRA::f3dVec            _dummy;
  std::vector<std::string> _dummy2;
};
}  // namespace VIPRA::Obstacles

CHECK_MODULE(ObstacleModule, VIPRA::Obstacles::QuadTree);