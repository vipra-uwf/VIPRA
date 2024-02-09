
#include "conditions/subconditions/subcondition_elapsed_time.hpp"

namespace {
inline constexpr auto in_time_step(VIPRA::time_s currTime, VIPRA::time_s checkTime, VIPRA::delta_t deltaT)
    -> bool {
  const VIPRA::delta_t left = checkTime - deltaT;
  const VIPRA::delta_t right = checkTime + deltaT;
  return (currTime > left && currTime < right);
}
}  // namespace

namespace BHVR {
void SubConditionElapsedTimeFromEvent::operator()(Simpack pack, const VIPRA::idxVec& peds,
                                                  const std::vector<Target>& /*unused*/,
                                                  std::vector<bool>& met, const std::vector<bool>& /*unused*/,
                                                  BoolOp /*unused*/) {
  for (auto ped : peds) {
    const auto& event = pack.get_context().events[_event];
    if (event.is_starting()) {
      _startTime = pack.get_context().elapsedTime;
    }

    float reqTime = _requiredTime.value(ped);
    float checkTime = _startTime + reqTime;
    met[ped] = in_time_step(pack.get_context().elapsedTime, checkTime, pack.dT);
  }
}
}  // namespace BHVR
