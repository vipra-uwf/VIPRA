#pragma once

#define VIPRA_MODEL_TIMESTEP                                                                    \
  template <VIPRA::Concepts::PedsetModule peds_t, VIPRA::Concepts::MapModule map_t,             \
            VIPRA::Concepts::GoalsModule goals_t, VIPRA::Concepts::BaseOutput output_t>         \
  void timestep(peds_t const& pedset, map_t const& map, goals_t const& goals, output_t& output, \
                VIPRA::State& state, VIPRA::delta_t deltaT, VIPRA::timestep timestep)

#define VIPRA_MODEL_INIT_STEP                                       \
  void initialize(const VIPRA::Concepts::PedsetModule auto& pedset, \
                  const VIPRA::Concepts::MapModule auto&    map,    \
                  const VIPRA::Concepts::GoalsModule auto&  goals,  \
                  const VIPRA::Concepts::BaseOutput auto& output, VIPRA::Random::Engine& engine)
