#pragma once

#include <stdexcept>
#include "vipra/macros/goals.hpp"
#include "vipra/modules/goals.hpp"

#include "vipra/geometry/f3d.hpp"
#include "vipra/types/float.hpp"
#include "vipra/types/idx.hpp"

namespace VIPRA::Views {
class Goals : public Modules::Goals<Goals> {
  using currentgoals_func_t = VIPRA::f3dVec const&(void const*);
  using endgoals_func_t = VIPRA::f3dVec const&(void const*);
  using pedcurrentgoal_func_t = VIPRA::f3d const&(void const*, VIPRA::idx);
  using pedendgoal_func_t = VIPRA::f3d const&(void const*, VIPRA::idx);
  using pedgoalmet_func_t = bool(void const*, VIPRA::idx);
  using simgoalmet_func_t = bool(void const*);
  using timesince_func_t = VIPRA::f_pnt(void const*, VIPRA::idx);

 private:
  void const* _module;

  currentgoals_func_t*   _currentgoals;
  endgoals_func_t*       _endgoals;
  pedcurrentgoal_func_t* _pedcurrentgoal;
  pedendgoal_func_t*     _pedendgoal;
  pedgoalmet_func_t*     _pedgoalmet;
  simgoalmet_func_t*     _simgoalmet;
  timesince_func_t*      _timesince;

 public:
  // NOLINTBEGIN
  VIPRA_GOALS_INIT_STEP { throw std::runtime_error("Goals View Should NOT Init"); }
  VIPRA_GOALS_UPDATE_STEP { throw std::runtime_error("Goals View Should NOT Update"); }
  // NOLINTEND

  template <typename module_t>
  // NOLINTNEXTLINE(google-explicit-constructor)
  Goals(module_t const& module) : _module(&module)
  {
    // NOLINTBEGIN(cppcoreguidelines-prefer-member-initializer)
    _currentgoals = +[](void const* ptr) -> VIPRA::f3dVec const& {
      return static_cast<module_t const*>(ptr)->current_goals();
    };

    _endgoals = +[](void const* ptr) -> VIPRA::f3dVec const& {
      return static_cast<module_t const*>(ptr)->end_goals();
    };

    _pedcurrentgoal = +[](void const* ptr, VIPRA::idx pedIdx) -> VIPRA::f3d const& {
      return static_cast<module_t const*>(ptr)->current_goal(pedIdx);
    };

    _pedendgoal = +[](void const* ptr, VIPRA::idx pedIdx) -> VIPRA::f3d const& {
      return static_cast<module_t const*>(ptr)->end_goal(pedIdx);
    };

    _pedgoalmet = +[](void const* ptr, VIPRA::idx pedIdx) {
      return static_cast<module_t const*>(ptr)->is_goal_met(pedIdx);
    };

    _simgoalmet = +[](void const* ptr) { return static_cast<module_t const*>(ptr)->is_sim_goal_met(); };

    _timesince = +[](void const* ptr, VIPRA::idx pedIdx) {
      return static_cast<module_t const*>(ptr)->time_since_last_goal(pedIdx);
    };
    // NOLINTEND(cppcoreguidelines-prefer-member-initializer)
  }

  void change_end_goal_impl(VIPRA::idx /*pedIdx*/, VIPRA::f3d /*currPos*/, VIPRA::f3d /*goalPos*/)
  {
    throw std::runtime_error("Goals View Should NOT Change end goals");
  }

  [[nodiscard]] auto current_goals() const -> VIPRA::f3dVec const& { return _currentgoals(_module); }

  [[nodiscard]] auto end_goals() const -> VIPRA::f3dVec const& { return _endgoals(_module); }

  [[nodiscard]] auto current_goal(VIPRA::idx pedIdx) const -> VIPRA::f3d const&
  {
    return _pedcurrentgoal(_module, pedIdx);
  }

  [[nodiscard]] auto end_goal(VIPRA::idx pedIdx) const -> VIPRA::f3d const&
  {
    return _pedendgoal(_module, pedIdx);
  }

  [[nodiscard]] auto is_goal_met(VIPRA::idx pedIdx) const -> bool { return _pedgoalmet(_module, pedIdx); }

  [[nodiscard]] auto is_sim_goal_met() const -> bool { return _simgoalmet(_module); }

  [[nodiscard]] auto time_since_last_goal(VIPRA::idx pedIdx) const -> VIPRA::f_pnt
  {
    return _timesince(_module, pedIdx);
  }
};
}  // namespace VIPRA::Views