#ifndef VIPRA_BEHAVIORS_SIM_PACK_HPP
#define VIPRA_BEHAVIORS_SIM_PACK_HPP

#include <functional>

#include "behavior_context.hpp"
#include "selectors/pedestrian_groups.hpp"
#include "vipra/concepts/goals.hpp"
#include "vipra/concepts/obstacle_set.hpp"
#include "vipra/concepts/pedset.hpp"

#include "vipra/geometry/f3d.hpp"

#include "vipra/types/state.hpp"
#include "vipra/types/time.hpp"

namespace BHVR {
/**
  * @brief Holds references to commonly used parameters for simpler passing
  * 
  */
template <VIPRA::Concepts::PedsetModule pedset_t, VIPRA::Concepts::ObstacleModule obsset_t,
          VIPRA::Concepts::GoalsModule goals_t>
struct Simpack {
  pedset_t const&        pedSet;
  obsset_t const&        obsSet;
  goals_t&               goals;
  VIPRA::State&          state;
  BehaviorContext&       context;
  GroupsContainer const& groups;
  VIPRA::delta_t         dT;
};
}  // namespace BHVR

#endif