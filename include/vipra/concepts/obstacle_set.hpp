#pragma once

#include <concepts>
#include <map>
#include <utility>
#include <vector>

#include "vipra/concepts/module.hpp"
#include "vipra/geometry/circle.hpp"
#include "vipra/geometry/polygon.hpp"
#include "vipra/modules.hpp"

#include "vipra/geometry/f3d.hpp"
#include "vipra/types/float.hpp"

namespace VIPRA::Concepts {
/**
 * @brief Checks that a type can be initialized following the obstacle set module interface
 * 
 * @tparam obstacle_t 
 */
template <typename obstacle_t>
concept can_initialize_obstacles = requires(obstacle_t                                            obsset,
                                            std::vector<VIPRA::Geometry::Polygon> const&          obstacles,
                                            std::vector<std::string> const&                       types,
                                            std::map<std::string, std::vector<VIPRA::f3d>> const& objects) {
  {obsset.initialize(obstacles, types, objects)};
};

/**
 * @brief Checks that a type has a get_objects method
 * 
 * @tparam obstacle_t 
 */
template <typename obstacle_t>
concept can_get_objects = requires(const obstacle_t obsset, std::string const& type) {
  { obsset.get_object_types() } -> std::same_as<std::vector<std::string> const&>;
  { obsset.get_objects(type) } -> std::same_as<const VIPRA::f3dVec&>;
};

/**
 * @brief Checks that a type has collision methods
 * 
 * @tparam obstacle_t 
 */
template <typename obstacle_t>
concept has_collisions = requires(const obstacle_t obsset) {
  { obsset.collision(VIPRA::f3d{}) } -> std::same_as<bool>;
  { obsset.collision(VIPRA::Geometry::Circle{VIPRA::f3d{}, 1.0F}) } -> std::same_as<bool>;
  { obsset.ray_hit(VIPRA::f3d{}, VIPRA::f3d{}) } -> std::same_as<VIPRA::f_pnt>;
};

// TODO(rolland): returning an f3d means we expect every map to start at the origin
/**
 * @brief Checks that a type has a get_dimensions method
 * 
 * @tparam obstacle_t 
 */
template <typename obstacle_t>
concept can_get_dimensions = requires(obstacle_t obsset) {
  { obsset.get_dimensions() } -> std::same_as<VIPRA::f3d>;
};

/**
 * @brief Checks that a type is an obstacle set module
 * 
 * @tparam obstacle_t 
 */
template <typename obstacle_t>
concept ObstacleModule =
    is_module<obstacle_t, VIPRA::Modules::Type::OBSTACLES> && can_initialize_obstacles<obstacle_t> &&
    can_get_objects<obstacle_t> && has_collisions<obstacle_t> && can_get_dimensions<obstacle_t>;

/**
 * @brief Dummy obstacle set for use in other concepts
 * 
 */
class DummyObsSet {
  // NOLINTBEGIN
 public:
  constexpr static VIPRA::Modules::Type _VIPRA_MODULE_TYPE_ = VIPRA::Modules::Type::OBSTACLES;

  void initialize(std::vector<VIPRA::Geometry::Polygon> const&, std::vector<std::string> const&,
                  std::map<std::string, std::vector<VIPRA::f3d>> const&) {}

  template <VIPRA::Concepts::ParamModule params_t>
  void register_params(params_t&) {}
  template <VIPRA::Concepts::ParamModule params_t>
  void config(params_t const&, VIPRA::Random::Engine&) {}

  // NOLINTBEGIN rolland: Dummy Object that cannot be used, we don't care about errors here
  auto get_dimensions() const -> VIPRA::f3d { return VIPRA::f3d{}; }
  auto collision(VIPRA::f3d) const -> bool { return false; }
  auto collision(VIPRA::Geometry::Circle) const -> bool { return false; }
  auto ray_hit(VIPRA::f3d, VIPRA::f3d) const -> VIPRA::f_pnt { return 1.0F; }
  auto get_object_types() const -> std::vector<std::string> const& { return _dummy2; }
  auto get_objects(std::string const&) const -> const VIPRA::f3dVec& { return _dummy; }
  // NOLINTEND

 private:
  VIPRA::f3dVec            _dummy;
  std::vector<std::string> _dummy2;
  DummyObsSet() = default;
  // NOLINTEND
};

CHECK_MODULE(ObstacleModule, DummyObsSet)

}  // namespace VIPRA::Concepts