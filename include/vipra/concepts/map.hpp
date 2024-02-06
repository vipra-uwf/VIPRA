#pragma once

#include "vipra/concepts/module.hpp"
#include "vipra/concepts/obstacle_set.hpp"
#include "vipra/concepts/pedset.hpp"

#include "vipra/macros/module.hpp"
#include "vipra/macros/parameters.hpp"
#include "vipra/modules.hpp"

namespace VIPRA::Concepts {

template <typename map_t>
concept can_initialize_map = requires(map_t map, const DummyPedSet& pedset) {
  {map.initialize(pedset)};
};

template <typename map_t>
concept can_get_obstacles = requires(const map_t map) {
  {map.obstacle_set()};
};

// TODO(rolland): add in requirement to match obstacle types
template <typename map_t>
concept MapModule =
    is_module<map_t, Modules::Type::MAP> && can_initialize_map<map_t> && can_get_obstacles<map_t>;

class DummyMap {
  // NOLINTBEGIN

 public:
  constexpr static VIPRA::Modules::Type _VIPRA_MODULE_TYPE_ = VIPRA::Modules::Type::MAP;

  template <VIPRA::Concepts::ParamModule params_t>
  void register_params(params_t& params) {}

  template <VIPRA::Concepts::ParamModule params_t>
  void config(const params_t& params) {}

  template <Concepts::PedsetModule pedestrians_t>
  void initialize(const pedestrians_t& pedestrians) {}

  auto obstacle_set() const -> const DummyObsSet& { return _dummy; }

 private:
  DummyObsSet _dummy;
  // NOLINTEND
};

CHECK_MODULE(MapModule, DummyMap);
}  // namespace VIPRA::Concepts