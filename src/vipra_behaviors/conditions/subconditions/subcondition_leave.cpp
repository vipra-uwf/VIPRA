#pragma once

#include "vipra/vipra_behaviors/conditions/subconditions/subcondition_leave.hpp"

namespace VIPRA::Behaviors {

void SubConditionLeave::operator()(Simpack pack, const VIPRA::idxVec& peds,
                                   std::vector<Target> const& targets,
                                   std::vector<bool>&         met,
                                   std::vector<bool> const& /*unused*/,
                                   BoolOp /*unused*/)
{
  for ( auto idx : peds ) {
    if ( _left.size() < pack.pedset.num_pedestrians() )
      _left.resize(pack.pedset.num_pedestrians());

    if ( _left[targets[idx].targetIdx] ) {
      met[idx] = false;
    }

    Location& loc = pack.context.locations[_location];
    bool leave = ! loc.contains(pack.state.positions[targets[idx].targetIdx]) &&
                 loc.contains(pack.pedset.ped_coords(targets[idx].targetIdx));

    if ( leave ) {
      _left[targets[idx].targetIdx] = true;
    }

    met[idx] = leave;
  }
}

}  // namespace VIPRA::Behaviors
