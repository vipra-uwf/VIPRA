#pragma once

#define VIPRA_FIELD_INIT_STEP                                                         \
  template <VIPRA::Concepts::PedsetModule pedset_t, VIPRA::Concepts::MapModule map_t> \
  void initialize(const pedset_t& pedset, const map_t& map)