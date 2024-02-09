#include <numeric>
#include <randomization/random.hpp>

#include "selectors/selector_everyone.hpp"

namespace BHVR {
/**
 * @brief Selects all pedestrians from a group
 * 
 * @param group : group to pull from
 * @return SelectorResult 
 */
auto SelectorEveryone::operator()(const VIPRA::idxVec& /*unused*/, const VIPRA::idxVec& group,
                                  Simpack /*unused*/) -> SelectorResult {
  auto vec = VIPRA::idxVec(group.size());
  std::iota(vec.begin(), vec.end(), 0);
  return {false, vec};
}

}  // namespace BHVR
