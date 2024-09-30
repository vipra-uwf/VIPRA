#pragma once

#include "vipra/vipra_behaviors/actions/action.hpp"

namespace VIPRA::Behaviors {

void Action::perform_action(Simpack pack, VIPRA::idxVec& peds,
                            std::vector<Target> const& targets)
{
  std::vector<bool> conditionMet;
  std::for_each(_atoms.begin(), _atoms.end(),
                [&](Atom& atom) { atom(pack, peds, conditionMet, targets); });
}
void Action::perform_action(Simpack pack, const VIPRA::idxVec& peds,
                            std::vector<bool> const&   conditionMet,
                            std::vector<Target> const& targets)
{
  std::for_each(_atoms.begin(), _atoms.end(),
                [&](Atom& atom) { atom(pack, peds, conditionMet, targets); });
}

}  // namespace VIPRA::Behaviors