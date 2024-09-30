#pragma once

#include "vipra/vipra_behaviors/conditions/subconditions/subcondition_exists.hpp"

namespace VIPRA::Behaviors {

void SubConditionExists::operator()(Simpack pack, const VIPRA::idxVec& peds,
                                    std::vector<Target> const& /*unused*/,
                                    std::vector<bool>&       met,
                                    std::vector<bool> const& prevMet,
                                    BoolOp                   oper) const
{
  for ( auto selfidx : peds ) {
    if ( short_circuit(selfidx, met, prevMet, oper) ) continue;

    met[selfidx] = false;
    auto const& group = pack.groups.at(0);

    for ( auto idx : pack.groups.at(0) ) {
      if ( _modifier.check(pack, selfidx, idx) &&
           _attributeCond.individual(pack, selfidx,
                                     {TargetType::PEDESTRIAN, idx}) ) {
        met[selfidx] = true;
        break;
      }
    }
  }
}

}  // namespace VIPRA::Behaviors