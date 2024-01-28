#pragma once

#include <concepts>
#include <utility>

#include "vipra/concepts/goals.hpp"
#include "vipra/concepts/map.hpp"
#include "vipra/concepts/obstacle_set.hpp"
#include "vipra/concepts/pedset.hpp"

#include "vipra/modules.hpp"

#include "vipra/randomization/random.hpp"

#include "vipra/types/state.hpp"
#include "vipra/types/time.hpp"

namespace VIPRA::Concepts {

template <typename model_t>
concept can_initialize_model = requires(model_t model, const DummyPedSet& pedset, const DummyMap& map,
                                        const DummyGoals& goals) {
  {model.initialize(pedset, map, goals)};
};

template <typename model_t>
concept has_model_timestep = requires(model_t model, const DummyPedSet& pedset, const DummyMap& map,
                                      const DummyGoals& goals, VIPRA::delta_t deltaT,
                                      VIPRA::timestep timestep) {
  { model.timestep(pedset, map, goals, deltaT, timestep) } -> std::same_as<const VIPRA::State&>;
};

template <typename model_t>
concept ModelModule = is_module<model_t, VIPRA::Modules::Type::MODEL> && has_model_timestep<model_t> &&
    can_initialize_model<model_t>;

class DummyModel {
  // NOLINTBEGIN
 public:
  constexpr static VIPRA::Modules::Type _VIPRA_MODULE_TYPE_ = VIPRA::Modules::Type::MODEL;

  template <typename params_t>
  void register_params(params_t&) {}

  template <typename pedset_t, typename map_t, typename goals_t>
  void initialize(const pedset_t&, const map_t&, const goals_t&) {}

  void config(auto&) {}

  auto timestep(const DummyPedSet&, const DummyMap&, const DummyGoals&, VIPRA::delta_t, VIPRA::timestep)
      -> const VIPRA::State& {
    return _state;
  }

 private:
  VIPRA::State _state;
  // NOLINTEND
};

CHECK_MODULE(ModelModule, DummyModel);

}  // namespace VIPRA::Concepts