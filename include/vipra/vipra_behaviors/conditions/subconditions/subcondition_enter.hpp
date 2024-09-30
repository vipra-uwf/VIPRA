#pragma once

#include "vipra/geometry/f3d.hpp"

#include "vipra/vipra_behaviors/conditions/sub_condition.hpp"
#include "vipra/vipra_behaviors/definitions/dsl_types.hpp"
#include "vipra/vipra_behaviors/definitions/sim_pack.hpp"
#include "vipra/vipra_behaviors/events/event.hpp"
#include "vipra/vipra_behaviors/locations/location.hpp"
#include "vipra/vipra_behaviors/time/time.hpp"
#include "vipra/vipra_behaviors/util/class_types.hpp"
#include "vipra/vipra_behaviors/util/timed_latch.hpp"
#include "vipra/vipra_behaviors/values/numeric_value.hpp"

namespace VIPRA::Behaviors {
class SubConditionEnter {
  NON_DEFAULT_CONSTRUCTIBLE(SubConditionEnter)
  COPYABLE(SubConditionEnter)
  MOVEABLE(SubConditionEnter)

 public:
  explicit SubConditionEnter(VIPRA::idx location) : _location{location} {}

  void operator()(Simpack pack, const VIPRA::idxVec& peds,
                  std::vector<Target> const& targets, std::vector<bool>& met,
                  std::vector<bool> const& /*unused*/, BoolOp /*unused*/)
  {
    if ( _entered.size() < pack.pedset.num_pedestrians() )
      _entered.resize(pack.pedset.num_pedestrians());

    for ( auto ped : peds ) {
      if ( _entered[targets[ped].targetIdx] ) {
        met[ped] = false;
      }

      Location& loc = pack.context.locations[_location];
      bool      enter =
          loc.contains(pack.state.positions[targets[ped].targetIdx]) &&
          ! loc.contains(pack.pedset.ped_coords(targets[ped].targetIdx));

      if ( enter ) {
        _entered[targets[ped].targetIdx] = true;
      }

      met[ped] = enter;
    }
  }

 private:
  VIPRA::idx _location;

  // TODO(rolland): temporary fix for not having pedestrian memory
  std::vector<bool> _entered;
};
}  // namespace VIPRA::Behaviors