#ifndef VIPRA_BHVR_SUB_SELECTOR_HPP
#define VIPRA_BHVR_SUB_SELECTOR_HPP

#include <functional>
#include <optional>

#include "goals/goals.hpp"
#include "obstacle_set/obstacle_set.hpp"
#include "pedestrian_set/pedestrian_set.hpp"

#include "conditions/sub_condition.hpp"

#include "definitions/behavior_context.hpp"
#include "definitions/pedestrian_types.hpp"
#include "definitions/sim_pack.hpp"
#include "randomization/random.hpp"
#include "selectors/pedestrian_groups.hpp"

namespace BHVR {

/**
 * @brief Result of a selection
 * 
 */
struct SelectorResult {
  bool          starved;
  VIPRA::idxVec group;
};

/**
 * @brief Function for choosing which pedestrians have a type
 * 
 */
using SelectorFunc = std::function<SelectorResult(const VIPRA::idxVec&, const VIPRA::idxVec&, Simpack)>;

/**
 * @brief Selects pedestrians for one type, gets combined with other SubSelectors in Selector
 * 
 */
class SubSelector {
  NON_DEFAULT_CONSTRUCTIBLE(SubSelector)
  COPYABLE(SubSelector)
  MOVEABLE(SubSelector)

 public:
  SubSelector(typeUID, Ptype, bool, SelectorFunc);

  auto select_peds(const VIPRA::idxVec&, const VIPRA::idxVec&, Simpack) -> SelectorResult;

  // NOLINTBEGIN - (rolland) Having these public increases readability   : ignoring (cppcoreguidelines-non-private-member-variables-in-classes)
  typeUID group;
  Ptype   type;
  bool    required;
  // NOLINTEND

 private:
  SelectorFunc                _select;
  std::optional<SubCondition> _condition;
};

}  // namespace BHVR

#endif