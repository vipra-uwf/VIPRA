#pragma once

#include "vipra/modules/map.hpp"
#include "vipra/modules/pedestrians.hpp"

#include "vipra/modules/goals.hpp"

#include "vipra/types/state.hpp"
#include "vipra/types/time.hpp"

#include "vipra/vipra_behaviors/selectors/pedestrian_groups.hpp"

namespace VIPRA::Behaviors {

struct BehaviorContext;

// NOLINTBEGIN(cppcoreguidelines-avoid-const-or-ref-data-members)
/**
  * @brief Holds references to commonly used parameters for simpler passing
  * 
  */

struct Simpack {
  Modules::Pedestrians const& pedset;
  Modules::Map const&         map;
  Modules::Goals&             goals;
  GroupsContainer const&      groups;
  BehaviorContext&            context;
  VIPRA::State&               state;
  VIPRA::delta_t              dT;
};
// NOLINTEND(cppcoreguidelines-avoid-const-or-ref-data-members)
}  // namespace VIPRA::Behaviors
