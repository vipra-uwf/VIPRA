#pragma once

#include <utility>

#include "vipra/modules/module.hpp"

#include "vipra/random/random.hpp"

#include "vipra/types/state.hpp"
#include "vipra/types/time.hpp"

namespace VIPRA::Modules {
/**
 * @brief Dummy model for use in other concepts
 * 
 */
template <typename class_t>
class Model : public Module<Model<class_t>> {
 public:
  template <typename pedset_t, typename map_t, typename goals_t, typename output_t>
  void initialize(pedset_t const& pedset, map_t const& map, goals_t const& goals, output_t& output,
                  VIPRA::Random::Engine& engine) {
    this->self().init_step(pedset, map, goals, output, engine);
  }

  template <typename pedset_t, typename map_t, typename goals_t, typename output_t>
  void take_timestep(pedset_t const& pedset, map_t const& map, goals_t const& goals, output_t const& output,
                     VIPRA::State& state, VIPRA::delta_t deltaT, VIPRA::timestep timestep) {
    this->self().timestep(pedset, map, goals, output, state, deltaT, timestep);
  }
};

}  // namespace VIPRA::Modules