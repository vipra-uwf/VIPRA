#pragma once

#include "vipra/macros/performance.hpp"

#define VIPRA_PEDS_INIT_STEP void init_step(auto const& input, auto const& obstacles)

#define VIPRA_PEDS_UPDATE_STEP VIPRA_PERF_FUNC void update_step(VIPRA::State const& state)
