#pragma once

#include <concepts>
#include <utility>

#include "vipra/concepts/goals.hpp"
#include "vipra/concepts/map.hpp"
#include "vipra/concepts/obstacle_set.hpp"
#include "vipra/concepts/pedset.hpp"
#include "vipra/modules.hpp"
#include "vipra/types/state.hpp"
#include "vipra/types/time.hpp"

namespace VIPRA::Concepts {

template <typename model_t>
concept can_initialize_model = requires(model_t model, const DummyPedSet& pedset) {
  {model.initialize(pedset)};
};

template <typename model_t>
concept has_model_timestep = requires(model_t model, const DummyPedSet& pedset, const DummyMap& map,
                                      const DummyGoals& goals, VIPRA::delta_t deltaT) {
  { model.timestep(pedset, map, goals, deltaT) } -> std::same_as<const VIPRA::State&>;
};

template <typename model_t>
concept ModelModule = is_module<model_t, VIPRA::Modules::Type::MODEL> && has_model_timestep<model_t> &&
    can_initialize_model<model_t>;

class DummyModel {
  // NOLINTBEGIN
 public:
  constexpr static VIPRA::Modules::Type MODULE_TYPE = VIPRA::Modules::Type::MODEL;

  template <typename params_t>
  void register_params(params_t&) {}

  template <typename pedset_t>
  void initialize(const pedset_t& /*unused*/) {}

  void config(auto& /*unused*/) {}

  auto timestep(const DummyPedSet& /*unused*/, const DummyMap& /*unused*/, const DummyGoals& /*unused*/,
                VIPRA::delta_t /*unused*/) -> const VIPRA::State& {
    return _state;
  }

 private:
  VIPRA::State _state;
  // NOLINTEND
};

CHECK_MODULE(ModelModule, DummyModel);

}  // namespace VIPRA::Concepts