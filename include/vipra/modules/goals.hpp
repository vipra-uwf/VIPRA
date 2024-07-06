#pragma once

#include <concepts>
#include <type_traits>

#include "vipra/geometry/f3d.hpp"
#include "vipra/modules/module.hpp"
#include "vipra/types/idx.hpp"
#include "vipra/types/time.hpp"

namespace VIPRA::Modules {
/**
 * @brief Goals module mixin
 * 
 */
template <typename module_t>
class Goals : public Module<Goals<module_t>> {
 public:
  template <typename pedset_t, typename map_t>
  void initialize(pedset_t const& pedset, map_t const& map) {
    return this->self().init_step(pedset, map);
  }

  template <typename pedset_t, typename map_t>
  void update(pedset_t const& pedset, map_t const& map, VIPRA::delta_t deltaT) {
    return this->self().update_step(pedset, map, deltaT);
  }

  void change_end_goal(VIPRA::idx pedIdx, VIPRA::f3d goalPos) {
    return this->self().change_end_goal(pedIdx, goalPos);
  }

  [[nodiscard]] auto current_goals() const -> const VIPRA::f3dVec& { return this->self().current_goals(); }

  [[nodiscard]] auto end_goals() const -> const VIPRA::f3dVec& { return this->self().end_goals(); }

  [[nodiscard]] auto current_goal(VIPRA::idx pedIdx) const -> VIPRA::f3d const& {
    return this->self().current_goal(pedIdx);
  }

  [[nodiscard]] auto end_goal(VIPRA::idx pedIdx) const -> VIPRA::f3d const& {
    return this->self().end_goal(pedIdx);
  }

  [[nodiscard]] auto is_goal_met(VIPRA::idx pedIdx) const -> bool { return this->self().is_goal_met(pedIdx); }

  [[nodiscard]] auto is_sim_goal_met() const -> bool { return this->self().is_sim_goal_met(); }

  [[nodiscard]] auto time_since_last_goal(VIPRA::idx pedIdx) const -> VIPRA::f_pnt {
    return this->self().time_since_last_goal(pedIdx);
  }
};

}  // namespace VIPRA::Modules
