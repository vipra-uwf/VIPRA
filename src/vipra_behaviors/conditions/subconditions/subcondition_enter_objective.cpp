

#include "vipra/vipra_behaviors/conditions/subconditions/subcondition_enter_objective.hpp"
#include "vipra/vipra_behaviors/definitions/behavior_context.hpp"
#include "vipra/vipra_behaviors/definitions/sim_pack.hpp"
#include "vipra/vipra_behaviors/locations/location.hpp"

namespace VIPRA::Behaviors {

void SubConditionEnterObj::operator()(Simpack pack, const VIPRA::idxVec& peds,
                                      std::vector<Target> const& targets,
                                      std::vector<bool>&         met,
                                      std::vector<bool> const& /*unused*/,
                                      BoolOp /*unused*/)
{
  if ( _entered.size() < pack.pedset.num_pedestrians() )
    _entered.resize(pack.pedset.num_pedestrians(), -1);

  for ( auto ped : peds ) {
    if ( _entered[targets[ped].targetIdx] < pack.dT - 1 ) {
      met[ped] = false;
    }

    auto const& objectives = pack.context.objectives[_objective];

    for ( auto const& objective : objectives ) {
      bool enter = objective.is_point_inside(
                       pack.state.positions[targets[ped].targetIdx]) &&
                   ! objective.is_point_inside(
                       pack.pedset.ped_coords(targets[ped].targetIdx));

      met[ped] = enter;

      if ( enter ) {
        _entered[targets[ped].targetIdx] = pack.dT;
        break;
      }
    }
  }
}

}  // namespace VIPRA::Behaviors