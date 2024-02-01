#pragma once

#define VIPRA_MODEL_TIMESTEP                                                                    \
  template <VIPRA::Concepts::PedsetModule peds_t, VIPRA::Concepts::MapModule map_t,             \
            VIPRA::Concepts::GoalsModule goals_t, VIPRA::Concepts::BaseOutput output_t>         \
  auto timestep(const peds_t& pedset, const map_t& map, const goals_t& goals, output_t& output, \
                VIPRA::delta_t deltaT, VIPRA::timestep timestep)                                \
      ->const VIPRA::State&

#define VIPRA_MODEL_INIT_STEP                                                                       \
  void initialize(                                                                                  \
      const VIPRA::Concepts::PedsetModule auto& pedset, const VIPRA::Concepts::MapModule auto& map, \
      const VIPRA::Concepts::GoalsModule auto& goals, const VIPRA::Concepts::BaseOutput auto& output)
