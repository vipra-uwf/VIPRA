#pragma once

#include <concepts>
#include "vipra/concepts/module.hpp"
#include "vipra/concepts/obstacle_set.hpp"
#include "vipra/concepts/pedset.hpp"

#include "vipra/macros/module.hpp"
#include "vipra/macros/parameters.hpp"
#include "vipra/modules.hpp"

namespace VIPRA::Concepts {

/**
 * @brief Checks that a type can be initialized following the map module interface
 * 
 * @tparam map_t 
 */
template <typename map_t>
concept can_initialize_map = requires(map_t map, DummyPedSet const& pedset) {
  {map.initialize(pedset)};
};

/**
 * @brief Checks that a type has an obstacle set method
 * 
 * @tparam map_t 
 */
template <typename map_t>
concept can_get_obstacles = requires(const map_t map) {
  {map.obstacle_set()};
};

// TODO(rolland): add in requirement to match obstacle types
/**
 * @brief Checks that a type is a map module
 * 
 * @tparam map_t 
 */
template <typename map_t>
concept MapModule =
    is_module<map_t, Modules::Type::MAP> && can_initialize_map<map_t> && can_get_obstacles<map_t>;

/**
 * @brief Dummy map for use in other concepts
 * 
 */
class DummyMap {
  // NOLINTBEGIN

 public:
  DummyMap() = default;
  constexpr static VIPRA::Modules::Type _VIPRA_MODULE_TYPE_ = VIPRA::Modules::Type::MAP;

  template <VIPRA::Concepts::ParamModule params_t>
  void register_params(params_t& params) {}

  template <VIPRA::Concepts::ParamModule params_t>
  void config(params_t const& params, VIPRA::Random::Engine&) {}

  template <Concepts::PedsetModule pedestrians_t>
  void initialize(pedestrians_t const& pedestrians) {}

  auto obstacle_set() const -> DummyObsSet const& { return _dummy; }

 private:
  DummyObsSet _dummy;
  // NOLINTEND
};

CHECK_MODULE(MapModule, DummyMap);
}  // namespace VIPRA::Concepts