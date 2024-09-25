
#pragma once

#include "vipra/macros/performance.hpp"

#define VIPRA_MODEL_TIMESTEP                                                                  \
  template <typename peds_t, typename map_t, typename goals_t>                                \
  VIPRA_PERF_FUNC void timestep(peds_t const& pedset, map_t const& map, goals_t const& goals, \
                                VIPRA::State& state, VIPRA::delta_t deltaT, VIPRA::timestep timestep)

#define VIPRA_MODEL_INIT_STEP                                                    \
  template <typename pedset_t, typename map_t, typename goals_t>                 \
  void init_step(pedset_t const& pedset, map_t const& map, goals_t const& goals, \
                 VIPRA::Random::Engine& engine)
