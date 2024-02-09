
#include "conditions/subconditions/subcondition_leave.hpp"

namespace BHVR {

void SubConditionLeave::operator()(Simpack pack, const VIPRA::idxVec& peds,
                                   const std::vector<Target>& targets, std::vector<bool>& met,
                                   const std::vector<bool>& /*unused*/, BoolOp /*unused*/) {
  for (auto idx : peds) {
    if (_left.size() < pack.get_pedset().getNumPedestrians())
      _left.resize(pack.get_pedset().getNumPedestrians());

    if (_left[targets[idx].targetIdx]) {
      met[idx] = false;
    }

    Location& loc = pack.get_context().locations[_location];
    bool      leave = !loc.contains(pack.get_state().coords[targets[idx].targetIdx]) &&
                 loc.contains(pack.get_pedset().getPedCoords(targets[idx].targetIdx));

    if (leave) {
      _left[targets[idx].targetIdx] = true;
    }

    met[idx] = leave;
  }
}

}  // namespace BHVR
