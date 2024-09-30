#pragma once

#include "vipra/vipra_behaviors/definitions/behavior_context.hpp"
#include "vipra/vipra_behaviors/definitions/pedestrian_types.hpp"

#include "vipra/vipra_behaviors/definitions/sim_pack.hpp"
#include "vipra/vipra_behaviors/selectors/pedestrian_groups.hpp"
#include "vipra/vipra_behaviors/selectors/subselector.hpp"

namespace VIPRA::Behaviors {

/**
 * @brief Combines/Organizes SubSelectors to select pedestrians for types
 * 
 */
class Selector {
  DEFAULT_CONSTRUCTIBLE(Selector)
  COPYABLE(Selector)
  MOVEABLE(Selector)

 public:
  void initialize(std::string const& /*behaviorName*/, Simpack pack);

  void set_all_types(Ptype types) { _allTypes = types; }
  void add_sub_selector(SubSelector const& subselector)
  {
    _subSelectors.push_back(subselector);
  }

  [[nodiscard]] auto get_groups() -> GroupsContainer& { return _pedGroups; }
  [[nodiscard]] auto get_groups() const -> GroupsContainer const&
  {
    return _pedGroups;
  }

  [[nodiscard]] auto selector_count() const -> VIPRA::size
  {
    return _subSelectors.size();
  }

 private:
  Ptype                    _allTypes;
  std::vector<SubSelector> _subSelectors;
  GroupsContainer          _pedGroups;

  [[nodiscard]] auto select_peds_from_group(
      SubSelector& /*selector*/, Simpack pack,
      std::string const& behaviorName) -> VIPRA::idxVec;

  static void check_for_duplicates(const VIPRA::idxVec& order);

  [[nodiscard]] auto        order_selectors() -> VIPRA::idxVec;
  [[nodiscard]] static auto filter_used_peds(const VIPRA::idxVec& /*peds*/,
                                             std::vector<bool> const& /*used*/)
      -> VIPRA::idxVec;

  void run_selectors(const VIPRA::idxVec& /*selectorIdxs*/,
                     std::string const& /*behaviorName*/, auto pack);
  void update_ped_groups(const VIPRA::idxVec& /*selectedPeds*/,
                         SubSelector& /*selector*/,
                         BehaviorContext& /*context*/,
                         std::string const& /*unused*/);
  void sort_groups();
};

}  // namespace VIPRA::Behaviors
