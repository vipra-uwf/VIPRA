
#include <algorithm>

#include "conditions/condition.hpp"
#include "conditions/sub_condition.hpp"
#include "util/timed_latch.hpp"

namespace VIPRA::Behaviors {

void Condition::initialize(Simpack const& pack) {
  if (_conditions.size() > 1) {
    _temp.resize(pack.get_pedset().getNumPedestrians());
    std::fill(_temp.begin(), _temp.end(), false);
  }
}

/**
 * @brief Tests whether a condition is met, by running through each sub condition sequentially
 * 
 * @param pedSet : pedestrian set object
 * @param obsSet : obstacle set object
 * @param goals : goals object
 * @param context : behavior context
 * @param pedIndex : index of current pedestrian
 * @param dT : simulation timestep size
 * @return true 
 * @return false
 */
void Condition::evaluate(Simpack pack, const VIPRA::idxVec& peds, std::vector<bool>& met,
                         std::vector<Target> const& targets, std::optional<TimedLatchCollection>&) {
  std::fill(_temp.begin(), _temp.end(), false);
  _conditions[0](pack, peds, targets, met, _temp, BoolOp::OR);

  for (VIPRA::idx i = 1; i < _conditions.size(); ++i) {
    _conditions[i](pack, peds, targets, _temp, met, _steps[i - 1]);
    switch (_steps[i - 1]) {
      case BoolOp::AND:
        std::transform(met.begin(), met.end(), _temp.begin(), met.begin(), std::logical_and<>());
        break;
      case BoolOp::OR:
        std::transform(met.begin(), met.end(), _temp.begin(), met.begin(), std::logical_or<>());
        break;
    }
  }
}
}  // namespace VIPRA::Behaviors