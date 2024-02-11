#pragma once

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
  void initialize(std::string const&, auto pack);

  void set_all_types(Ptype);
  void add_sub_selector(select_t const&);

  [[nodiscard]] auto get_groups() -> GroupsContainer&;

  [[nodiscard]] auto selector_count() const -> VIPRA::size;

 private:
  Ptype                 _allTypes;
  std::vector<select_t> _subSelectors;
  GroupsContainer       _pedGroups;

  [[nodiscard]] auto select_peds_from_group(select_t&, auto pack, std::string const&) -> VIPRA::idxVec;

  [[nodiscard]] auto        order_selectors() -> VIPRA::idxVec;
  [[nodiscard]] static auto filter_used_peds(const VIPRA::idxVec&, std::vector<bool> const&) -> VIPRA::idxVec;

  void run_selectors(const VIPRA::idxVec&, std::string const&, auto pack);
  void update_used_peds(const VIPRA::idxVec&, std::vector<bool>&);
  void update_ped_groups(const VIPRA::idxVec&, select_t&, context_t&, std::string const&);
  void sort_groups();
};
}  // namespace VIPRA::Behaviors
