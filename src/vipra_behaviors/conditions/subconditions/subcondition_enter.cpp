#pragma once

#include "vipra/vipra_behaviors/conditions/subconditions/subcondition_enter.hpp"

namespace VIPRA::Behaviors {

void SubConditionEnter::operator()(Simpack pack, const VIPRA::idxVec& peds,
                                   std::vector<Target> const& targets,
                                   std::vector<bool>&         met,
                                   std::vector<bool> const& /*unused*/,
                                   BoolOp /*unused*/)
{
  if ( _entered.size() < pack.pedset.num_pedestrians() )
    _entered.resize(pack.pedset.num_pedestrians());

  for ( auto ped : peds ) {
    if ( _entered[targets[ped].targetIdx] ) {
      met[ped] = false;
    }

    Location& loc = pack.context.locations[_location];
    bool enter = loc.contains(pack.state.positions[targets[ped].targetIdx]) &&
                 ! loc.contains(pack.pedset.ped_coords(targets[ped].targetIdx));

    if ( enter ) {
      _entered[targets[ped].targetIdx] = true;
    }

    met[ped] = enter;
  }
}

}  // namespace VIPRA::Behaviors