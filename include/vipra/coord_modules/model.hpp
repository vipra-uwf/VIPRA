#pragma once

#include <tuple>

#include "vipra/concepts/goals.hpp"
#include "vipra/concepts/model.hpp"
#include "vipra/concepts/parameters.hpp"
#include "vipra/concepts/pedset.hpp"

// TODO(rolland): figure out if this should be able to hold more than one model type

namespace VIPRA::Module {
template <Concepts::ModelModule model_t>
class Model {
  VIPRA_MODULE_TYPE(MODEL);

 public:
  constexpr explicit Model(model_t models) : _model(models) {}

  template <Concepts::ParamModule params_t>
  static void register_params() {
    model_t::template register_params<params_t>();
  }

  template <VIPRA::Concepts::PedsetModule peds_t, VIPRA::Concepts::MapModule map_t,
            VIPRA::Concepts::GoalsModule goals_t>
  auto timestep(const peds_t& pedset, const map_t& obsset, const goals_t& goals) -> const VIPRA::State& {
    return _model.timestep(pedset, obsset, goals);
  }

  template <Concepts::ParamModule params_t>
  void setup(const params_t& params) {
    _model.setup(params);
  }

  template <Concepts::PedsetModule pedset_t>
  void initialize(const pedset_t& pedset) {
    _model.initialize(pedset);
  }

  // TODO(rolland): this will cause the models to both be applied to the same pedset and obstacles
  template <Concepts::PedsetModule pedset_t, Concepts::ObstacleModule obs_t>
  void timestep(const pedset_t& pedset, const obs_t& obstacles) {
    _model.timestep(pedset, obstacles);
  }

 private:
  model_t _model;
};

CHECK_MODULE(ModelModule, Model<Concepts::DummyModel>);
}  // namespace VIPRA::Module