#pragma once

#include <functional>
#include <optional>

#include "vipra/vipra_behaviors/conditions/sub_condition.hpp"
#include "vipra/vipra_behaviors/definitions/behavior_context.hpp"
#include "vipra/vipra_behaviors/definitions/pedestrian_types.hpp"
#include "vipra/vipra_behaviors/definitions/sim_pack.hpp"
#include "vipra/vipra_behaviors/selectors/pedestrian_groups.hpp"

namespace VIPRA::Behaviors {

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
template <typename pack_t>
using SelectorFunc = std::function<SelectorResult(const VIPRA::idxVec&, const VIPRA::idxVec&, pack_t)>;

/**
 * @brief Selects pedestrians for one type, gets combined with other SubSelectors in Selector
 * 
 */
template <typename selector_t, typename cond_t>
class SubSelector {
  NON_DEFAULT_CONSTRUCTIBLE(SubSelector)
  COPYABLE(SubSelector)
  MOVEABLE(SubSelector)

 public:
  SubSelector(typeUID grp, Ptype typ, bool req, selector_t sel)
      : group(grp), type(typ), required(req), _select(std::move(sel)) {}

  auto select_peds(const VIPRA::idxVec& totalGroup, const VIPRA::idxVec& usableGroup, auto pack)
      -> SelectorResult {
    return _select(totalGroup, usableGroup, pack);
  }

  // NOLINTBEGIN - (rolland) Having these public increases readability   : ignoring (cppcoreguidelines-non-private-member-variables-in-classes)
  typeUID group;
  Ptype   type;
  bool    required;
  // NOLINTEND

 private:
  selector_t            _select;
  std::optional<cond_t> _condition;
};

}  // namespace VIPRA::Behaviors
