#pragma once

#include <concepts>
#include <utility>

#include "vipra/concepts/pedset.hpp"
#include "vipra/modules.hpp"

namespace VIPRA::Concepts {

template <typename obstacle_t>
concept Obstacle_Objects = requires(obstacle_t obsset, const std::string& type) {
  { obsset.get_object_types() } -> std::same_as<const std::vector<std::string>&>;
  { obsset.get_objects(type) } -> std::same_as<const VIPRA::f3dVec&>;
};

template <typename obstacle_t>
concept Obstacle_Get_Nearest = requires(obstacle_t obsset) {
  { obsset.nearest_obstacle(VIPRA::f3d{}) } -> std::same_as<VIPRA::f3d>;
  { obsset.nearest_obstacle_in_direction(VIPRA::f3d{}, VIPRA::f3d{}) } -> std::same_as<VIPRA::f3d>;
};

template <typename obstacle_t>
concept Obstacle_Questions = requires(obstacle_t obsset) {
  { obsset.collision(VIPRA::f3d{}) } -> std::same_as<bool>;
  { obsset.ray_hit(VIPRA::f3d{}, VIPRA::f3d{}) } -> std::same_as<float>;
};

template <typename obstacle_t>
concept Obstacle_Map = requires(obstacle_t obsset) {
  { obsset.get_map_dimensions() } -> std::same_as<std::pair<VIPRA::f3d, VIPRA::f3d>>;
};

template <typename obstacle_t>
concept ObstacleModule =
    Module<obstacle_t, VIPRA::Modules::Type::OBSTACLES> && Obstacle_Objects<obstacle_t> &&
    Obstacle_Get_Nearest<obstacle_t> && Obstacle_Questions<obstacle_t> && Obstacle_Map<obstacle_t>;

class DummyObsSet {
  VIPRA_MODULE_TYPE(OBSTACLES);

 public:
  void set_params(auto& /*unused*/) {}

  // NOLINTBEGIN rolland: Dummy Object that cannot be used, we don't care about errors here
  auto get_map_dimensions() -> std::pair<VIPRA::f3d, VIPRA::f3d> {}
  auto collision(VIPRA::f3d) -> bool {}
  auto ray_hit(VIPRA::f3d, VIPRA::f3d) -> float {}
  auto nearest_obstacle(VIPRA::f3d) -> VIPRA::f3d {}
  auto nearest_obstacle_in_direction(VIPRA::f3d, VIPRA::f3d) -> VIPRA::f3d {}
  auto get_object_types() -> const std::vector<std::string>& {}
  auto get_objects(const std::string&) -> const VIPRA::f3dVec& {}
  // NOLINTEND

 private:
  VIPRA::f3dVec _dummy;
  DummyObsSet() = default;
};

static_assert(ObstacleModule<DummyObsSet>);

}  // namespace VIPRA::Concepts