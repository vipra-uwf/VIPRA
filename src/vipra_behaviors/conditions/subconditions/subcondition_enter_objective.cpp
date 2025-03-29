

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
  if ( _visited.size() < pack.pedset.num_pedestrians() )
    _visited.resize(pack.pedset.num_pedestrians());

  for ( auto ped : peds ) {
    auto const& objectives = pack.context.objectives[_objective];

    for ( size_t i = 0; i < objectives.size(); ++i ) {
      if ( _visited[ped].contains(i) ) {
        met[ped] = false;
        continue;
      }

      bool inside =
          objectives[i].is_point_inside(pack.state.positions[targets[ped].targetIdx]);

      if ( inside ) {
        _visited[ped].insert(i);
        met[ped] = true;
        break;
      }
    }
  }
}
}  // namespace VIPRA::Behaviors