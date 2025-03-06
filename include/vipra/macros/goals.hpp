#pragma once

#include "vipra/macros/performance.hpp"

#define VIPRA_GOALS_INIT_STEP                               \
  void init_step(VIPRA::Modules::Pedestrians const& pedset, \
                 VIPRA::Modules::Map const& map, VIPRA::Random::Engine& engine)

#define VIPRA_GOALS_UPDATE_STEP                               \
  void update_step(VIPRA::Modules::Pedestrians const& pedset, \
                   VIPRA::Modules::Map const& map, VIPRA::delta_t deltaT)

#define VIPRA_GOALS_NEXT_GOAL                                                  \
  auto next_goal(VIPRA::idx pedIdx, VIPRA::Modules::Pedestrians const& pedset, \
                 VIPRA::Modules::Map const& map, VIPRA::delta_t deltaT) -> bool

#define VIPRA_GOALS_CHANGE_GOAL                                                    \
  void change_end_goal_impl(VIPRA::idx pedIdx, VIPRA::f3d pos, VIPRA::f3d newGoal, \
                            VIPRA::Random::Engine& engine)

#define VIPRA_GOALS_RESET void reset_module()