#pragma once

#include "vipra/macros/performance.hpp"

#define VIPRA_PEDS_INIT_STEP \
  void init_step(VIPRA::Modules::Map const& map, VIPRA::Random::Engine& engine)

#define VIPRA_PEDS_UPDATE_STEP void update_step(VIPRA::State const& state)

#define VIPRA_PEDS_RESET void reset_peds_module()