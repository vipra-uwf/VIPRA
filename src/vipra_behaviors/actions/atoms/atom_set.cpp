
#include "vipra/vipra_behaviors/actions/atoms/atom_set.hpp"

namespace VIPRA::Behaviors {
void AtomSet::operator()(Simpack pack, const VIPRA::idxVec& peds,
                         std::vector<bool> const&   conditionMet,
                         std::vector<Target> const& targets) const
{
  for ( VIPRA::idx idx = 0; idx < peds.size(); ++idx ) {
    if ( conditionMet[idx] ) {
      AttributeHandling::set_value(targets[idx], attribute, pack, pack.state,
                                   value);
    }
  }
}

}  // namespace VIPRA::Behaviors