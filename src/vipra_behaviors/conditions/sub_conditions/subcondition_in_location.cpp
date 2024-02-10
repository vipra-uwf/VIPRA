#include "conditions/subconditions/subcondition_in_location.hpp"

namespace VIPRA::Behaviors {

void SubConditionInLocation::operator()(Simpack pack, const VIPRA::idxVec& peds,
                                        std::vector<Target> const& targets, std::vector<bool>& met,
                                        std::vector<bool> const& /*unused*/, BoolOp /*unused*/) const {
  for (auto idx : peds) {
    auto const& coords = pack.get_pedset().getPedCoords(targets[idx].targetIdx);
    met[idx] = pack.get_context().locations[_location].contains(coords);
  }
}

}  // namespace VIPRA::Behaviors