
#include "vipra/vipra_behaviors/selectors/selector.hpp"
#include "vipra/vipra_behaviors/behavior/exceptions.hpp"

namespace VIPRA::Behaviors {
/**
 * @brief Initializes the pedestrian groups, and runs the sub selectors over them
 * 
 * @param behaviorName : 
 * @param seed : 
 * @param pedSet : 
 * @param map : 
 * @param goals : 
 */
void Selector::initialize(std::string const& behaviorName, Simpack pack)
{
  _pedGroups.initialize(_allTypes, pack.pedset.num_pedestrians());

  auto selectorIdxs = order_selectors();
  run_selectors(selectorIdxs, behaviorName, pack);
  sort_groups();
  _pedGroups.clean_used();
}

/**
 * @brief Goes through each sub selector in the provided order, updating the pedestrian groups
 * 
 * @param selectorIdxs : 
 * @param behaviorName : 
 * @param seed : 
 * @param context : 
 * @param pedSet : 
 * @param map : 
 * @param goals : 
 */
void Selector::run_selectors(const VIPRA::idxVec& selectorIdxs,
                             std::string const& behaviorName, auto pack)
{
  std::for_each(
      selectorIdxs.begin(), selectorIdxs.end(), [&](VIPRA::idx index) {
        auto& selector = _subSelectors[index];
        auto  selectedPeds =
            select_peds_from_group(selector, pack, behaviorName);
        update_ped_groups(selectedPeds, selector, pack.context, behaviorName);
      });

  _pedGroups[0].resize(pack.pedset.num_pedestrians());
  std::iota(_pedGroups[0].begin(), _pedGroups[0].end(), 0);
}

/**
 * @brief Selects pedestrians using a sub selector
 * 
 * @param selector : 
 * @param seed : 
 * @param pedSet : 
 * @param map : 
 * @param goals : 
 * @param behaviorName : 
 * @return VIPRA::idxVec 
 */
auto Selector::select_peds_from_group(SubSelector& selector, Simpack pack,
                                      std::string const& behaviorName)
    -> VIPRA::idxVec
{
  auto const& fullGroup = _pedGroups.get_group(selector.group);
  auto        usablegroup =
      filter_used_peds(fullGroup, _pedGroups.get_used(selector.group));
  auto result = selector.select_peds(fullGroup, usablegroup, pack);

  if ( ! result.starved ) {
    return result.group;
  }

  if ( selector.required ) {
    VIPRA::Log::error(
        "Behavior: {}, Required Selector Starved For Type: {} From Group: {}",
        behaviorName, selector.type.fullType, selector.group);
    DSLException::error();
  }

  VIPRA::Log::debug(
      "Behavior: {}, Starved Selector For Type: {} From Group: {}",
      behaviorName, selector.type.fullType, selector.group);
  return result.group;
}

/**
 * @brief Adds each selected pedestrian to their respective type groups, sets the pedestrian as used in the original group
 * 
 * @param selectedPeds : 
 * @param selector : 
 * @param context : 
 * @param behaviorName : 
 */
inline void Selector::update_ped_groups(const VIPRA::idxVec& selectedPeds,
                                        SubSelector&         selector,
                                        BehaviorContext&     context,
                                        std::string const&   behaviorName)
{
  std::for_each(selectedPeds.begin(), selectedPeds.end(), [&](auto& pedIdx) {
    selector.type.for_each_type([&](typeUID type) {
      VIPRA::Log::debug("Behavior: {}, Selecting Ped {} for Type: {}",
                        behaviorName, pedIdx, type);
      context.types[pedIdx] += type;
      _pedGroups.add_ped(pedIdx, type);
    });

    _pedGroups.set_used(pedIdx, selector.group);
  });
}

/**
 * @brief Returns a vector with all of the pedestrians from a group that are not already used
 * 
 * @param peds : 
 * @param used : 
 * @return VIPRA::idxVec 
 */
inline auto Selector::filter_used_peds(
    const VIPRA::idxVec& peds, std::vector<bool> const& used) -> VIPRA::idxVec
{
  VIPRA::idxVec ret;

  for ( VIPRA::idx i = 0; i < peds.size(); ++i ) {
    if ( ! used[i] ) {
      ret.push_back(peds[i]);
    }
  }

  return ret;
}

/**
 * @brief Sorts all of the groups in the selector's group container
 * 
 */
inline void Selector::sort_groups()
{
  const VIPRA::size groupCnt = _pedGroups.size();
  for ( VIPRA::idx i = 1; i < groupCnt; ++i ) {
    std::sort(_pedGroups[i].begin(), _pedGroups[i].end());
  }
}

/**
 * @brief Checks that there are no duplicate selectors in the order
 * @note if there is, something is very wrong
 * 
 * @param order : 
 */
inline void Selector::check_for_duplicates(const VIPRA::idxVec& order)
{
  for ( VIPRA::idx i = 0; i < order.size(); ++i ) {
    for ( VIPRA::idx j = i + 1; j < order.size(); ++j ) {
      if ( order[i] == order[j] ) {
        VIPRA::Log::error("Duplicate Selector in Selectors");
        throw std::runtime_error("");
      }
    }
  }
}

/**
 * @brief Stable sorts the selectors based on their group dependencies
 * 
 * @return VIPRA::idxVec 
 */
inline auto Selector::order_selectors() -> VIPRA::idxVec
{
  VIPRA::idxVec order;

  for ( VIPRA::idx selIdx = 0; selIdx < _subSelectors.size(); ++selIdx ) {
    if ( _subSelectors[selIdx].group == 0 ) {
      order.push_back(selIdx);
    }
  }

  _allTypes.for_each_type([&](typeUID type) {
    for ( VIPRA::idx selIdx = 0; selIdx < _subSelectors.size(); ++selIdx ) {
      if ( _subSelectors[selIdx].group == type ) {
        order.push_back(selIdx);
      }
    }
  });

  for ( VIPRA::idx selIdx = 0; selIdx < _subSelectors.size(); ++selIdx ) {
    bool notInGraph =
        std::find_if(order.begin(), order.end(), [&](VIPRA::idx index) {
          return index == selIdx;
        }) == order.end();
    if ( notInGraph ) {
      order.push_back(selIdx);
    }
  }

  check_for_duplicates(order);
  return order;
}
}  // namespace VIPRA::Behaviors