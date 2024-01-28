#pragma once

#define VIPRA_GOALS_INIT_STEP                                           \
  template <Concepts::PedsetModule pedset_t, Concepts::MapModule map_t> \
  void initialize(const pedset_t& pedset, const map_t& map)
