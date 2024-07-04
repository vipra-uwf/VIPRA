#pragma once

#define VIPRA_GOALS_INIT_STEP                                           \
  template <Concepts::PedsetModule pedset_t, Concepts::MapModule map_t> \
  void initialize(pedset_t const& pedset, map_t const& map)

#define VIPRA_GOALS_UPDATE                                              \
  template <Concepts::PedsetModule pedset_t, Concepts::MapModule map_t> \
  void update(pedset_t const& pedset, map_t const& /*unused*/, VIPRA::delta_t deltaT)
