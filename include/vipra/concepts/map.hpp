#pragma once

#include "vipra/concepts/module.hpp"
#include "vipra/concepts/obstacle_set.hpp"
#include "vipra/concepts/pedset.hpp"
#include "vipra/modules.hpp"

namespace VIPRA::Concepts {

template <typename map_t>
concept can_initialize_map = requires(map_t map, const DummyPedSet& pedset) {
  {map.initialize(pedset)};
};

// TODO(rolland): add in requirement to match obstacle types
template <typename map_t>
concept MapModule = is_module<map_t, Modules::Type::MAP> && can_initialize_map<map_t>;

class DummyMap {
  VIPRA_MODULE_TYPE(MAP);

 public:
  template <typename params_t>
  static void register_params() {}

  void setup(auto& params) {}

  template <Concepts::PedsetModule pedestrians_t>
  void initialize(const pedestrians_t& pedestrians) {}
};

CHECK_MODULE(MapModule, DummyMap);
}  // namespace VIPRA::Concepts