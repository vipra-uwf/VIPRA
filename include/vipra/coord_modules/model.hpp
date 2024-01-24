#pragma once

#include <tuple>

#include "vipra/concepts/model.hpp"
#include "vipra/concepts/parameters.hpp"
#include "vipra/concepts/pedset.hpp"

namespace VIPRA::Module {
template <Concepts::ModelModule... model_ts>
class Model {
  VIPRA_MODULE_TYPE(MODEL);

 public:
  constexpr explicit Model(model_ts... models) : _models(std::make_tuple(models...)) {}

  template <Concepts::ParamModule params_t>
  static void register_params() {
    (model_ts::template register_params<params_t>(), ...);
  }

  template <VIPRA::Concepts::PedsetModule peds_t, VIPRA::Concepts::MapModule map_t>
  void timestep(const peds_t& pedset, const map_t& obsset) {
    std::apply([&](auto&&... models) { (models.template timestep<peds_t, map_t>(pedset, obsset), ...); },
               _models);
  }

  template <Concepts::ParamModule params_t>
  void setup(const params_t& params) {
    std::apply([&](auto&&... models) { (models.template setup<params_t>(params), ...); }, _models);
  }

  template <Concepts::PedsetModule pedset_t>
  void initialize(const pedset_t& pedset) {
    std::apply([&](auto&&... models) { (models.template initialize<pedset_t>(pedset), ...); }, _models);
  }

  // TODO(rolland): this will cause the models to both be applied to the same pedset and obstacles
  template <Concepts::PedsetModule pedset_t, Concepts::ObstacleModule obs_t>
  void timestep(const pedset_t& pedset, const obs_t& obstacles) {
    std::apply([&](auto&&... models) { (models.template timestep<pedset_t, obs_t>(pedset, obstacles), ...); },
               _models);
  }

 private:
  std::tuple<model_ts...> _models;
};

CHECK_MODULE(ModelModule, Model<Concepts::DummyModel>);
}  // namespace VIPRA::Module