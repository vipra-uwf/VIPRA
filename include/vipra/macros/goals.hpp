#pragma once

#include "vipra/types/time.hpp"

#include "vipra/macros/performance.hpp"

#define VIPRA_GOALS_INIT_STEP                        \
  template <typename pedset_t, typename obstacles_t> \
  void init_step(pedset_t const& pedset, obstacles_t const& obstacles)

#define VIPRA_GOALS_UPDATE_STEP                                                           \
  template <typename pedset_t, typename obstacles_t>                                      \
  VIPRA_PERF_FUNC void update_step(pedset_t const& pedset, obstacles_t const& /*unused*/, \
                                   VIPRA::delta_t  deltaT)
