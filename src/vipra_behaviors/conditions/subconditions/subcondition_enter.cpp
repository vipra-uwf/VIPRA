

#include "vipra/vipra_behaviors/conditions/subconditions/subcondition_enter.hpp"
#include "vipra/vipra_behaviors/definitions/behavior_context.hpp"
#include "vipra/vipra_behaviors/definitions/sim_pack.hpp"
#include "vipra/vipra_behaviors/locations/location.hpp"

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
      continue;
    }

    Location& loc = pack.context.locations[_location];
    bool      enter =
        loc.is_point_inside(pack.state.positions[targets[ped].targetIdx]) &&
        ! loc.is_point_inside(pack.pedset.ped_coords(targets[ped].targetIdx));

    if ( enter ) {
      _entered[targets[ped].targetIdx] = true;
    }

    met[ped] = enter;
  }
}

}  // namespace VIPRA::Behaviors