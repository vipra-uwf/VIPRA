#pragma once

#include <concepts>
#include <map>
#include <utility>

#include "vipra/concepts/pedset.hpp"
#include "vipra/modules.hpp"
#include "vipra/types/float.hpp"

namespace VIPRA::Concepts {

template <typename obstacle_t>
concept can_initialize_obstacles = requires(obstacle_t obsset, const std::vector<VIPRA::f3d>& obstacles,
                                            const std::vector<std::string>&                       types,
                                            const std::map<std::string, std::vector<VIPRA::f3d>>& objects) {
  {obsset.initialize(obstacles, types, objects)};
};

template <typename obstacle_t>
concept can_get_objects = requires(obstacle_t obsset, const std::string& type) {
  { obsset.get_object_types() } -> std::same_as<const std::vector<std::string>&>;
  { obsset.get_objects(type) } -> std::same_as<const VIPRA::f3dVec&>;
};

template <typename obstacle_t>
concept can_get_nearest_obs = requires(obstacle_t obsset) {
  { obsset.nearest_obstacle(VIPRA::f3d{}) } -> std::same_as<VIPRA::f3d>;
  { obsset.nearest_obstacle_in_direction(VIPRA::f3d{}, VIPRA::f3d{}) } -> std::same_as<VIPRA::f3d>;
};

template <typename obstacle_t>
concept has_collisions = requires(obstacle_t obsset) {
  { obsset.collision(VIPRA::f3d{}) } -> std::same_as<bool>;
  { obsset.ray_hit(VIPRA::f3d{}, VIPRA::f3d{}) } -> std::same_as<VIPRA::f_pnt>;
};

template <typename obstacle_t>
concept can_get_dimensions = requires(obstacle_t obsset) {
  { obsset.get_dimensions() } -> std::same_as<std::pair<VIPRA::f3d, VIPRA::f3d>>;
};

template <typename obstacle_t>
concept ObstacleModule =
    is_module<obstacle_t, VIPRA::Modules::Type::OBSTACLES> && can_get_objects<obstacle_t> &&
    can_get_nearest_obs<obstacle_t> && has_collisions<obstacle_t> && can_get_dimensions<obstacle_t>;

class DummyObsSet {
  VIPRA_MODULE_TYPE(OBSTACLES);

 public:
  template <typename params_t>
  static void register_params() {}

  void setup(auto& params) {}

  // NOLINTBEGIN rolland: Dummy Object that cannot be used, we don't care about errors here
  auto get_dimensions() -> std::pair<VIPRA::f3d, VIPRA::f3d> { return {VIPRA::f3d{}, VIPRA::f3d{}}; }
  auto collision(VIPRA::f3d) -> bool { return false; }
  auto ray_hit(VIPRA::f3d, VIPRA::f3d) -> VIPRA::f_pnt { return 1.0F; }
  auto nearest_obstacle(VIPRA::f3d) -> VIPRA::f3d { return VIPRA::f3d{}; }
  auto nearest_obstacle_in_direction(VIPRA::f3d, VIPRA::f3d) -> VIPRA::f3d { return VIPRA::f3d{}; }
  auto get_object_types() -> const std::vector<std::string>& { return _dummy2; }
  auto get_objects(const std::string&) -> const VIPRA::f3dVec& { return _dummy; }
  // NOLINTEND

 private:
  VIPRA::f3dVec            _dummy;
  std::vector<std::string> _dummy2;
  DummyObsSet() = default;
};

CHECK_MODULE(ObstacleModule, DummyObsSet)

}  // namespace VIPRA::Concepts