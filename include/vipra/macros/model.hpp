#pragma once

#define VIPRA_MODEL_TIMESTEP                                                                                \
  template <typename peds_t, typename obstacles_t, typename goals_t, typename output_t>                     \
  void timestep(peds_t const& pedset, obstacles_t const& obstacles, goals_t const& goals, output_t& output, \
                VIPRA::State& state, VIPRA::delta_t deltaT, VIPRA::timestep timestep)

#define VIPRA_MODEL_INIT_STEP                                                                \
  template <typename pedset_t, typename obstacles_t, typename goals_t, typename output_t>    \
  void init_step(pedset_t const& pedset, obstacles_t const& obstacles, goals_t const& goals, \
                 output_t& output, VIPRA::Random::Engine& engine)
