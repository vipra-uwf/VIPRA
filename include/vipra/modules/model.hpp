#pragma once

#include "vipra/random/random.hpp"

#include "vipra/types/state.hpp"
#include "vipra/types/time.hpp"
#include "vipra/util/crtp.hpp"

namespace VIPRA::Modules {
/**
 * @brief Dummy model for use in other concepts
 * 
 */
template <typename class_t>
class Model : public Util::CRTP<Model<class_t>> {
  using Util::CRTP<Model<class_t>>::self;

 public:
  template <typename pedset_t, typename obstacles_t, typename goals_t>
  void initialize(pedset_t const& pedset, obstacles_t const& obstacles, goals_t const& goals,
                  VIPRA::Random::Engine& engine)
  {
    self().init_step(pedset, obstacles, goals, engine);
  }

  template <typename pedset_t, typename obstacles_t, typename goals_t>
  void take_timestep(pedset_t const& pedset, obstacles_t const& obstacles, goals_t const& goals,
                     VIPRA::State& state, VIPRA::delta_t deltaT, VIPRA::timestep timestep)
  {
    self().timestep(pedset, obstacles, goals, state, deltaT, timestep);
  }
};

}  // namespace VIPRA::Modules