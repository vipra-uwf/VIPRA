#pragma once

#include "vipra/types/time.hpp"

#define VIPRA_GOALS_INIT_STEP                  \
  template <typename pedset_t, typename map_t> \
  void init(pedset_t const& pedset, map_t const& map)

#define VIPRA_GOALS_UPDATE_STEP                \
  template <typename pedset_t, typename map_t> \
  void update_step(pedset_t const& pedset, map_t const& /*unused*/, VIPRA::delta_t deltaT)
