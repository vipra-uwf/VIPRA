#pragma once

#include "vipra/concepts/module.hpp"
#include "vipra/concepts/obstacle_set.hpp"
#include "vipra/concepts/pedset.hpp"
#include "vipra/modules.hpp"

namespace VIPRA::Concepts {

template <typename field_t>
concept Field_Initialization = requires(field_t field, const DummyPedSet& pedset, const DummyObsSet& obsset) {
  {field.initialize(pedset, obsset)};
};

template <typename field_t>
concept FieldModule = Module<field_t, Modules::Type::FIELD> && Field_Initialization<field_t>;

class DummyField {
  VIPRA_MODULE_TYPE(FIELD);

 public:
  template <Concepts::ObstacleModule obstacles_t, Concepts::PedsetModule pedestrians_t>
  void initialize(const pedestrians_t& pedestrians, const obstacles_t& obstacles) {}

  template <typename params_t>
  static void register_params() {}

  void setup(auto& params) {}
};

CHECK_MODULE(FieldModule, DummyField)
}  // namespace VIPRA::Concepts