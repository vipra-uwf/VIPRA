#pragma once

#include <functional>

#include "vipra/concepts/goals.hpp"
#include "vipra/concepts/obstacle_set.hpp"
#include "vipra/concepts/pedset.hpp"

#include "vipra/geometry/f3d.hpp"

#include "vipra/types/state.hpp"
#include "vipra/types/time.hpp"

#include "vipra/vipra_behaviors/definitions/behavior_context.hpp"
#include "vipra/vipra_behaviors/selectors/pedestrian_groups.hpp"

namespace VIPRA::Behaviors {
// NOLINTBEGIN(cppcoreguidelines-avoid-const-or-ref-data-members)
/**
  * @brief Holds references to commonly used parameters for simpler passing
  * 
  */
template <VIPRA::Concepts::PedsetModule pedset_t, VIPRA::Concepts::ObstacleModule obsset_t,
          VIPRA::Concepts::GoalsModule goals_t, typename context_t>
struct Simpack {
  pedset_t const&        pedSet;
  obsset_t const&        obsSet;
  goals_t&               goals;
  VIPRA::State&          state;
  context_t&             context;
  GroupsContainer const& groups;
  VIPRA::delta_t         dT;
};
// NOLINTEND(cppcoreguidelines-avoid-const-or-ref-data-members)
}  // namespace VIPRA::Behaviors
