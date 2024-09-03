#pragma once

#include "vipra/random/random.hpp"

#include "vipra/types/state.hpp"
#include "vipra/types/time.hpp"
#include "vipra/util/crtp.hpp"

namespace VIPRA::Modules {
/**
 * @brief Base Model module
 * 
 */
template <typename class_t>
class Model : public Util::CRTP<Model<class_t>> {
  using Util::CRTP<Model<class_t>>::self;

 public:
  template <typename pedset_t, typename map_t, typename goals_t>
  void initialize(pedset_t const& pedset, map_t const& map, goals_t const& goals,
                  VIPRA::Random::Engine& engine)
  {
    self().init_step(pedset, map, goals, engine);
  }

  template <typename pedset_t, typename map_t, typename goals_t>
  void take_timestep(pedset_t const& pedset, map_t const& map, goals_t const& goals, VIPRA::State& state,
                     VIPRA::delta_t deltaT, VIPRA::timestep timestep)
  {
    self().timestep(pedset, map, goals, state, deltaT, timestep);
  }
};

}  // namespace VIPRA::Modules