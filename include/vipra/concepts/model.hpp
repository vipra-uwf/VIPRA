#pragma once

#include <concepts>
#include <utility>

#include "vipra/concepts/map.hpp"
#include "vipra/concepts/obstacle_set.hpp"
#include "vipra/concepts/pedset.hpp"
#include "vipra/modules.hpp"

namespace VIPRA::Concepts {

template <typename model_t>
concept Model_Initialization = requires(model_t model, const DummyPedSet& pedset) {
  {model.initialize(pedset)};
};

template <typename model_t>
concept Model_Timestep = requires(model_t model, const DummyPedSet& pedset, const DummyMap& map) {
  { model.timestep(pedset, map) } -> std::same_as<void>;
};

template <typename model_t>
concept ModelModule =
    Module<model_t, VIPRA::Modules::Type::MODEL> && Model_Timestep<model_t> && Model_Initialization<model_t>;

class DummyModel {
  VIPRA_MODULE_TYPE(MODEL)
 public:
  template <typename params_t>
  static void register_params() {}

  template <typename pedset_t>
  void initialize(const pedset_t& /*unused*/) {}

  void setup(auto& /*unused*/) {}

  void timestep(const DummyPedSet& /*unused*/, const DummyMap& /*unused*/) {}
};

CHECK_MODULE(ModelModule, DummyModel);

}  // namespace VIPRA::Concepts