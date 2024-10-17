
#pragma once

#define VIPRA_MODEL_INIT_STEP                                \
  void initialize(VIPRA::Modules::Pedestrians const& pedset, \
                  VIPRA::Modules::Map const&         map,    \
                  VIPRA::Modules::Goals const&       goals,  \
                  VIPRA::Random::Engine&             engine)

#define VIPRA_MODEL_TIMESTEP                                             \
  void timestep(VIPRA::Modules::Pedestrians const& pedset,               \
                VIPRA::Modules::Map const&         map,                  \
                VIPRA::Modules::Goals const& goals, VIPRA::State& state, \
                VIPRA::delta_t deltaT, VIPRA::timestep timestep)
