#pragma once

#include <stdexcept>

#include "behavior/exceptions.hpp"
#include "vipra/logging/logging.hpp"

#include "vipra/vipra_behaviors/actions/action.hpp"
#include "vipra/vipra_behaviors/definitions/behavior_context.hpp"
#include "vipra/vipra_behaviors/definitions/pedestrian_types.hpp"
#include "vipra/vipra_behaviors/selectors/pedestrian_groups.hpp"

namespace VIPRA::Behaviors {

/**
 * @brief Combines/Organizes SubSelectors to select pedestrians for types
 * 
 */
template <typename select_t, typename context_t>
class Selector {
  DEFAULT_CONSTRUCTIBLE(Selector)
  COPYABLE(Selector)
  MOVEABLE(Selector)

 public:
  void initialize(std::string const& /*behaviorName*/, auto pack);

  void set_all_types(Ptype types) { _allTypes = types; }
  void add_sub_selector(select_t const& subselector) { _subSelectors.push_back(subselector); }

  [[nodiscard]] auto get_groups() -> GroupsContainer& { return _pedGroups; }
  [[nodiscard]] auto get_groups() const -> GroupsContainer const& { return _pedGroups; }

  [[nodiscard]] auto selector_count() const -> VIPRA::size { return _subSelectors.size(); }

 private:
  Ptype                 _allTypes;
  std::vector<select_t> _subSelectors;
  GroupsContainer       _pedGroups;

  [[nodiscard]] auto select_peds_from_group(select_t& /*selector*/, auto pack,
                                            std::string const& behaviorName) -> VIPRA::idxVec;

  void check_for_duplicates(const VIPRA::idxVec& order);

  [[nodiscard]] auto        order_selectors() -> VIPRA::idxVec;
  [[nodiscard]] static auto filter_used_peds(const VIPRA::idxVec& /*peds*/, std::vector<bool> const& /*used*/)
      -> VIPRA::idxVec;

  void run_selectors(const VIPRA::idxVec& /*selectorIdxs*/, std::string const& /*behaviorName*/, auto pack);
  void update_ped_groups(const VIPRA::idxVec& /*selectedPeds*/, select_t& /*selector*/,
                         context_t& /*context*/, std::string const& /*unused*/);
  void sort_groups();
};

/**
 * @brief Initializes the pedestrian groups, and runs the sub selectors over them
 * 
 * @param behaviorName : 
 * @param seed : 
 * @param pedSet : 
 * @param obsSet : 
 * @param goals : 
 */
template <typename select_t, typename context_t>
void Selector<select_t, context_t>::initialize(std::string const& behaviorName, auto pack)
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
 * @param obsSet : 
 * @param goals : 
 */
template <typename select_t, typename context_t>
void Selector<select_t, context_t>::run_selectors(const VIPRA::idxVec& selectorIdxs,
                                                  std::string const& behaviorName, auto pack)
{
  std::for_each(selectorIdxs.begin(), selectorIdxs.end(), [&](VIPRA::idx index) {
    auto& selector = _subSelectors[index];
    auto  selectedPeds = select_peds_from_group(selector, pack, behaviorName);
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
 * @param obsSet : 
 * @param goals : 
 * @param behaviorName : 
 * @return VIPRA::idxVec 
 */
template <typename select_t, typename context_t>
auto Selector<select_t, context_t>::select_peds_from_group(select_t& selector, auto pack,
                                                           std::string const& behaviorName) -> VIPRA::idxVec
{
  auto const& fullGroup = _pedGroups.get_group(selector.group);
  auto        usablegroup = filter_used_peds(fullGroup, _pedGroups.get_used(selector.group));
  auto        result = selector.select_peds(fullGroup, usablegroup, pack);

  if ( ! result.starved ) {
    return result.group;
  }

  if ( selector.required ) {
    VIPRA::Log::error("Behavior: {}, Required Selector Starved For Type: {} From Group: {}", behaviorName,
                      selector.type.fullType, selector.group);
    DSLException::error();
  }

  VIPRA::Log::debug("Behavior: {}, Starved Selector For Type: {} From Group: {}", behaviorName,
                    selector.type.fullType, selector.group);
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
template <typename select_t, typename context_t>
void Selector<select_t, context_t>::update_ped_groups(const VIPRA::idxVec& selectedPeds, select_t& selector,
                                                      context_t& context, std::string const& behaviorName)
{
  std::for_each(selectedPeds.begin(), selectedPeds.end(), [&](auto& pedIdx) {
    selector.type.for_each_type([&](typeUID type) {
      VIPRA::Log::debug("Behavior: {}, Selecting Ped {} for Type: {}", behaviorName, pedIdx, type);
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
template <typename select_t, typename context_t>
auto Selector<select_t, context_t>::filter_used_peds(const VIPRA::idxVec& peds, std::vector<bool> const& used)
    -> VIPRA::idxVec
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
template <typename select_t, typename context_t>
void Selector<select_t, context_t>::sort_groups()
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
template <typename select_t, typename context_t>
void Selector<select_t, context_t>::check_for_duplicates(const VIPRA::idxVec& order)
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
template <typename select_t, typename context_t>
auto Selector<select_t, context_t>::order_selectors() -> VIPRA::idxVec
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
    bool notInGraph = std::find_if(order.begin(), order.end(),
                                   [&](VIPRA::idx index) { return index == selIdx; }) == order.end();
    if ( notInGraph ) {
      order.push_back(selIdx);
    }
  }

  check_for_duplicates(order);
  return order;
}

}  // namespace VIPRA::Behaviors
