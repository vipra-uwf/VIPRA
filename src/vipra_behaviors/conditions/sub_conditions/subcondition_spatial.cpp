#include "conditions/subconditions/subcondition_spatial.hpp"

namespace BHVR {

void SubConditionSpatial::operator()(Simpack pack, const VIPRA::idxVec& peds,
                                     const std::vector<Target>& targets, std::vector<bool>& met,
                                     const std::vector<bool>& /*unused*/, BoolOp /*unused*/) const {
  for (auto idx : peds) {
    const auto& coords = pack.get_pedset().getCoordinates();
    met[idx] = coords[idx].distanceTo(coords[targets[idx].targetIdx]) <= _within.value(idx);
  }
}
}  // namespace BHVR
