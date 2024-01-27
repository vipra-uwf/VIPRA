#pragma once

#include <concepts>
#include <type_traits>

#include "vipra/concepts/map.hpp"
#include "vipra/concepts/module.hpp"
#include "vipra/concepts/pedset.hpp"
#include "vipra/macros/parameters.hpp"
#include "vipra/modules.hpp"

namespace VIPRA::Concepts {

template <typename goals_t>
concept can_get_goals = requires(const goals_t goals) {
  { goals.current_goals() } -> std::same_as<const std::vector<VIPRA::f3d>&>;
  { goals.end_goals() } -> std::same_as<const std::vector<VIPRA::f3d>&>;
  { goals.current_goal(VIPRA::idx{}) } -> std::same_as<VIPRA::f3d>;
  { goals.end_goal(VIPRA::idx{}) } -> std::same_as<VIPRA::f3d>;
  { goals.is_goal_met(VIPRA::idx{}) } -> std::same_as<bool>;
  { goals.is_sim_goal_met() } -> std::same_as<bool>;
};

template <typename goals_t>
concept can_initialize_goals = requires(goals_t goals, const DummyPedSet& pedset, const DummyMap& map) {
  {goals.initialize(pedset, map)};
};

template <typename goals_t>
concept can_update_goals = requires(goals_t goals, const DummyPedSet& pedset, const DummyMap& map) {
  {goals.update(pedset, map)};
};

template <typename goals_t>
concept GoalsModule = is_module<goals_t, VIPRA::Modules::Type::GOALS> && can_initialize_goals<goals_t> &&
    can_get_goals<goals_t> && can_update_goals<goals_t>;

class DummyGoals {
  // NOLINTBEGIN
 public:
  constexpr static VIPRA::Modules::Type _VIPRA_MODULE_TYPE_ = VIPRA::Modules::Type::GOALS;

  template <VIPRA::Concepts::ParamModule params_t>
  void register_params(params_t& params) {}

  template <VIPRA::Concepts::ParamModule params_t>
  void config(const params_t& params) {}

  template <typename pedset_t, typename map_t>
  void initialize(const pedset_t& /*unused*/, const map_t& /*unused*/) {}

  template <typename pedset_t, typename map_t>
  void update(const pedset_t& /*unused*/, const map_t& /*unused*/) {}

  auto current_goals() const -> const VIPRA::f3dVec& { return _dummy; }
  auto end_goals() const -> const VIPRA::f3dVec& { return _dummy; }
  auto current_goal(VIPRA::idx) const -> VIPRA::f3d { return VIPRA::f3d{}; }
  auto end_goal(VIPRA::idx) const -> VIPRA::f3d { return VIPRA::f3d{}; }
  auto is_goal_met(VIPRA::idx) const -> bool { return false; }
  auto is_sim_goal_met() const -> bool { return false; }

 private:
  VIPRA::f3dVec _dummy;
  // NOLINTEND
};

CHECK_MODULE(GoalsModule, Concepts::DummyGoals);
}  // namespace VIPRA::Concepts
