#include "conditions/subconditions/subcondition_in_location.hpp"

namespace BHVR {

void SubConditionInLocation::operator()(Simpack pack, const VIPRA::idxVec& peds,
                                        const std::vector<Target>& targets, std::vector<bool>& met,
                                        const std::vector<bool>& /*unused*/, BoolOp /*unused*/) const {
  for (auto idx : peds) {
    const auto& coords = pack.get_pedset().getPedCoords(targets[idx].targetIdx);
    met[idx] = pack.get_context().locations[_location].contains(coords);
  }
}

}  // namespace BHVR