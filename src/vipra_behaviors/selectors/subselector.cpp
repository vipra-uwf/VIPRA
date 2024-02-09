

#include <selectors/subselector.hpp>

namespace BHVR {

/**
 * @brief Runs the select function to select pedestrians
 * 
 * @param rngEngine : randomization engine
 * @param totalGroup : the whole group the selector pulls from
 * @param usableGroup : the actual selectable pedestrians from that group
 * @param pedSet : pedestrian set
 * @param obsSet : obstacle set
 * @param goals : goals module
 * @return SelectorResult 
 */
auto SubSelector::select_peds(const VIPRA::idxVec& totalGroup, const VIPRA::idxVec& usableGroup, Simpack pack)
    -> SelectorResult {
  return _select(totalGroup, usableGroup, pack);
}

// ------------------- CONSTRUCTORS --------------------------------------------------------------------------

SubSelector::SubSelector(typeUID grp, Ptype typ, bool req, SelectorFunc sel)
    : group(grp), type(typ), required(req), _select(std::move(sel)) {}

}  // namespace BHVR