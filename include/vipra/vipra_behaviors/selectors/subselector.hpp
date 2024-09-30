#pragma once

#include <functional>
#include <optional>

#include "vipra/vipra_behaviors/conditions/condition.hpp"

#include "vipra/vipra_behaviors/definitions/pedestrian_types.hpp"
#include "vipra/vipra_behaviors/definitions/sim_pack.hpp"

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
using SelectorFunc = std::function<SelectorResult(
    const VIPRA::idxVec&, const VIPRA::idxVec&, Simpack)>;

/**
 * @brief Selects pedestrians for one type, gets combined with other SubSelectors in Selector
 * 
 */
class SubSelector {
  NON_DEFAULT_CONSTRUCTIBLE(SubSelector)
  COPYABLE(SubSelector)
  MOVEABLE(SubSelector)

 public:
  SubSelector(typeUID grp, Ptype typ, bool req, SelectorFunc sel)
      : group(grp), type(typ), required(req), _select(std::move(sel))
  {
  }

  auto select_peds(const VIPRA::idxVec& totalGroup,
                   const VIPRA::idxVec& usableGroup,
                   auto                 pack) -> SelectorResult
  {
    return _select(totalGroup, usableGroup, pack);
  }

  // NOLINTBEGIN - (rolland) Having these public increases readability   : ignoring (cppcoreguidelines-non-private-member-variables-in-classes)
  typeUID group;
  Ptype   type;
  bool    required;
  // NOLINTEND

 private:
  SelectorFunc             _select;
  std::optional<Condition> _condition;
};

}  // namespace VIPRA::Behaviors
