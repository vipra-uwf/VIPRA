
#include "conditions/subconditions/subcondition_leave.hpp"

namespace VIPRA::Behaviors {

void SubConditionLeave::operator()(Simpack pack, const VIPRA::idxVec& peds,
                                   std::vector<Target> const& targets, std::vector<bool>& met,
                                   std::vector<bool> const& /*unused*/, BoolOp /*unused*/) {
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

}  // namespace VIPRA::Behaviors
