#pragma once

#define VIPRA_MODEL_TIMESTEP                                                                    \
  template <typename peds_t, typename map_t, typename goals_t, typename output_t>               \
  void timestep(peds_t const& pedset, map_t const& map, goals_t const& goals, output_t& output, \
                VIPRA::State& state, VIPRA::delta_t deltaT, VIPRA::timestep timestep)

#define VIPRA_MODEL_INIT_STEP                                                                       \
  template <typename pedset_t, typename map_t, typename goals_t, typename output_t>                 \
  void initialize(pedset_t const& pedset, map_t const& map, goals_t const& goals, output_t& output, \
                  VIPRA::Random::Engine& engine)
