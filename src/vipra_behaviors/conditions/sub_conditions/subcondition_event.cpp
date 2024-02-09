
#include "conditions/subconditions/subcondition_event.hpp"

namespace BHVR {

void SubConditionEventOccurred::operator()(Simpack pack, const VIPRA::idxVec& /*unused*/,
                                           const std::vector<Target>& /*unused*/, std::vector<bool>& met,
                                           const std::vector<bool>& /*unused*/, BoolOp /*unused*/) const {
  std::fill(met.begin(), met.end(), pack.get_context().events[_event].has_occurred());
}
}  // namespace BHVR