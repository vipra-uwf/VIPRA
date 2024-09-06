#pragma once

#include "vipra/types/time.hpp"

#include "vipra/macros/performance.hpp"

#define VIPRA_GOALS_INIT_STEP                  \
  template <typename pedset_t, typename map_t> \
  void init_step(pedset_t const& pedset, map_t const& map)

#define VIPRA_GOALS_UPDATE_STEP                                                           \
  template <typename pedset_t, typename map_t>                                            \
  VIPRA_PERF_FUNC VIPRA_INLINE void update_step(pedset_t const& pedset, map_t const& map, \
                                                VIPRA::delta_t deltaT)

#define VIPRA_GOALS_NEXT_GOAL                                                                              \
  template <typename pedset_t, typename map_t>                                                             \
  VIPRA_PERF_FUNC VIPRA_INLINE auto next_goal(VIPRA::idx pedIdx, pedset_t const& pedset, map_t const& map, \
                                              VIPRA::delta_t deltaT) -> bool

#define VIPRA_GOALS_CHANGE_GOAL \
  void change_end_goal_impl(VIPRA::idx pedIdx, VIPRA::f3d pos, VIPRA::f3d newGoal)
