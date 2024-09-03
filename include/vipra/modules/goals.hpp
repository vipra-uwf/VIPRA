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
class Goals : public Util::CRTP<Goals<module_t>> {
  using Util::CRTP<Goals<module_t>>::self;

 public:
  template <typename pedset_t, typename map_t>
  void initialize(pedset_t const& pedset, map_t const& map)
  {
    return self().init_step(pedset, map);
  }

  template <typename pedset_t, typename map_t>
  void update(pedset_t const& pedset, map_t const& map, VIPRA::delta_t deltaT)
  {
    return self().update_step(pedset, map, deltaT);
  }

  void change_end_goal(VIPRA::idx pedIdx, VIPRA::f3d currPos, VIPRA::f3d goalPos)
  {
    return self().change_end_goal_impl(pedIdx, currPos, goalPos);
  }

  [[nodiscard]] auto current_goals() const -> const VIPRA::f3dVec& { return self().current_goals_impl(); }

  [[nodiscard]] auto end_goals() const -> const VIPRA::f3dVec& { return self().end_goals_impl(); }

  [[nodiscard]] auto current_goal(VIPRA::idx pedIdx) const -> VIPRA::f3d const&
  {
    return self().current_goal_impl(pedIdx);
  }

  [[nodiscard]] auto end_goal(VIPRA::idx pedIdx) const -> VIPRA::f3d const&
  {
    return self().end_goal_impl(pedIdx);
  }

  [[nodiscard]] auto is_goal_met(VIPRA::idx pedIdx) const -> bool { return self().is_goal_met_impl(pedIdx); }

  [[nodiscard]] auto is_sim_goal_met() const -> bool { return self().is_sim_goal_met_impl(); }

  [[nodiscard]] auto time_since_last_goal(VIPRA::idx pedIdx) const -> VIPRA::f_pnt
  {
    return self().time_since_last_goal_impl(pedIdx);
  }
};

}  // namespace VIPRA::Modules
