
#include "conditions/subconditions/subcondition_exists.hpp"

namespace BHVR {
void SubConditionExists::operator()(Simpack pack, const VIPRA::idxVec&                        peds,
                                    const std::vector<Target>& /*unused*/, std::vector<bool>& met,
                                    const std::vector<bool>& prevMet, BoolOp oper) const {
  for (auto selfidx : peds) {
    if (short_circuit(selfidx, met, prevMet, oper)) continue;

    met[selfidx] = false;
    const auto& group = pack.get_groups().at(0);

    for (auto idx : pack.get_groups().at(0)) {
      if (_modifier.check(pack, selfidx, idx) &&
          _attributeCond.individual(pack, selfidx, {TargetType::PEDESTRIAN, idx})) {
        met[selfidx] = true;
        break;
      }
    }
  }
}
}  // namespace BHVR