#pragma once

#include <concepts>
#include <utility>

#include "vipra/concepts/obstacle_set.hpp"
#include "vipra/concepts/pedset.hpp"
#include "vipra/modules.hpp"

namespace VIPRA::Concepts {

template <typename model_t>
concept Model_Initialization = requires(model_t model, const DummyPedSet& pedset) {
  {model.initialize(pedset)};
};

template <typename model_t>
concept ModelModule = Module<model_t, VIPRA::Modules::Type::MODEL> &&
    requires(model_t model, const DummyPedSet& pedset, const DummyObsSet& obsset) {
  { model.timestep(pedset, obsset) } -> std::same_as<void>;
};

class DummyModel {
  VIPRA_MODULE_TYPE(MODEL)
 public:
  template <typename params_t>
  static void register_params() {}

  template <typename pedset_t>
  void initialize(const pedset_t& /*unused*/) {}

  void setup(auto& /*unused*/) {}
  void timestep(const DummyPedSet& /*unused*/, const DummyObsSet& /*unused*/) {}
};

CHECK_MODULE(ModelModule, DummyModel);

}  // namespace VIPRA::Concepts