

#include "vipra/vipra_behaviors/conditions/subconditions/subcondition_elapsed_time.hpp"
#include "vipra/vipra_behaviors/definitions/behavior_context.hpp"
#include "vipra/vipra_behaviors/events/event.hpp"

namespace VIPRA::Behaviors {

void SubConditionElapsedTimeFromEvent::operator()(Simpack pack, const VIPRA::idxVec& peds,
                                                  std::vector<Target> const& /*unused*/,
                                                  std::vector<bool>& met,
                                                  std::vector<bool> const& /*unused*/,
                                                  BoolOp /*unused*/)
{
  for ( auto ped : peds ) {
    auto const& event = pack.context.events[_event];
    if ( event.is_starting() ) {
      _startTime = pack.context.elapsedTime;
    }

    VIPRA::f_pnt reqTime = _requiredTime.value(ped);
    VIPRA::f_pnt checkTime = _startTime + reqTime;
    met[ped] = in_time_step(pack.context.elapsedTime, checkTime, pack.dT);
  }
}

}  // namespace VIPRA::Behaviors