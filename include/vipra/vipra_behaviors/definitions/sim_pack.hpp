#pragma once

#include <functional>

#include "vipra/geometry/f3d.hpp"

#include "vipra/types/state.hpp"
#include "vipra/types/time.hpp"

#include "vipra/vipra_behaviors/selectors/pedestrian_groups.hpp"

namespace VIPRA::Behaviors {

template <typename pedset_t, typename map_t, typename goals_t>
struct BehaviorContext;

// NOLINTBEGIN(cppcoreguidelines-avoid-const-or-ref-data-members)
/**
  * @brief Holds references to commonly used parameters for simpler passing
  * 
  */
template <typename pedset_t, typename map_t, typename goals_t>
struct Simpack {
  pedset_t const&                            pedset;
  map_t const&                               obsset;
  goals_t&                                   goals;
  GroupsContainer const&                     groups;
  BehaviorContext<pedset_t, map_t, goals_t>& context;
  VIPRA::State&                              state;
  VIPRA::delta_t                             dT;
};
// NOLINTEND(cppcoreguidelines-avoid-const-or-ref-data-members)
}  // namespace VIPRA::Behaviors
