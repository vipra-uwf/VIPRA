#pragma once

#include "vipra/concepts/module.hpp"
#include "vipra/concepts/obstacle_set.hpp"
#include "vipra/concepts/pedset.hpp"
#include "vipra/macros/parameters.hpp"
#include "vipra/modules.hpp"

namespace VIPRA::Concepts {

template <typename field_t>
concept can_initialize_field = requires(field_t field, const DummyPedSet& pedset, const DummyObsSet& obsset) {
  {field.initialize(pedset, obsset)};
};

template <typename field_t>
concept FieldModule = is_module<field_t, Modules::Type::FIELD> && can_initialize_field<field_t>;

class DummyField {
  // NOLINTBEGIN

 public:
  constexpr static VIPRA::Modules::Type MODULE_TYPE = VIPRA::Modules::Type::FIELD;
  ;
  template <Concepts::ObstacleModule obstacles_t, Concepts::PedsetModule pedestrians_t>
  void initialize(const pedestrians_t& pedestrians, const obstacles_t& obstacles) {}

  template <VIPRA::Concepts::ParamModule params_t>
  void register_params(params_t& params) {}

  template <VIPRA::Concepts::ParamModule params_t>
  void config(const params_t& params) {}
  // NOLINTEND
};

CHECK_MODULE(FieldModule, DummyField)
}  // namespace VIPRA::Concepts