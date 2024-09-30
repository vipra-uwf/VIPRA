#pragma once

#include "vipra/vipra_behaviors/actions/atoms/atom_scale.hpp"

namespace VIPRA::Behaviors {

void AtomScale::operator()(Simpack pack, const VIPRA::idxVec& peds,
                           std::vector<bool> const&   conditionMet,
                           std::vector<Target> const& targets) const
{
  for ( VIPRA::idx idx = 0; idx < peds.size(); ++idx ) {
    if ( conditionMet[idx] )
      AttributeHandling::scale_value(targets[idx], attribute, pack, pack.state,
                                     value);
  }
}
}  // namespace VIPRA::Behaviors
