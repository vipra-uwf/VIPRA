
#include "conditions/subconditions/subcondition_event_starting.hpp"

namespace BHVR {
void SubConditionEventStarting::operator()(Simpack pack, const VIPRA::idxVec& /*unused*/,
                                           const std::vector<Target>& /*unused*/, std::vector<bool>& met,
                                           const std::vector<bool>& /*unused*/, BoolOp /*unused*/) const {
  std::fill(met.begin(), met.end(), pack.get_context().events[_event].is_starting());
}

}  // namespace BHVR