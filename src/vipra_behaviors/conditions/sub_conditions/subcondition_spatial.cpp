#include "conditions/subconditions/subcondition_spatial.hpp"

namespace VIPRA::Behaviors {

void SubConditionSpatial::operator()(Simpack pack, const VIPRA::idxVec& peds,
                                     std::vector<Target> const& targets, std::vector<bool>& met,
                                     std::vector<bool> const& /*unused*/, BoolOp /*unused*/) const {
  for (auto idx : peds) {
    auto const& coords = pack.get_pedset().getCoordinates();
    met[idx] = coords[idx].distanceTo(coords[targets[idx].targetIdx]) <= _within.value(idx);
  }
}
}  // namespace VIPRA::Behaviors
