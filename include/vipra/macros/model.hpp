
#pragma once

#include "vipra/modules/views/goals.hpp"
#include "vipra/modules/views/obstacles.hpp"
#include "vipra/modules/views/pedestrians.hpp"

#define VIPRA_MODEL_TIMESTEP                                                              \
  template <typename peds_t, typename obstacles_t, typename goals_t>                      \
  void timestep(peds_t const& pedset, obstacles_t const& obstacles, goals_t const& goals, \
                VIPRA::State& state, VIPRA::delta_t deltaT, VIPRA::timestep timestep)

#define VIPRA_MODEL_INIT_STEP                                                                \
  template <typename pedset_t, typename obstacles_t, typename goals_t>                       \
  void init_step(pedset_t const& pedset, obstacles_t const& obstacles, goals_t const& goals, \
                 VIPRA::Random::Engine& engine)
