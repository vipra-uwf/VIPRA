#pragma once

#include <concepts>
#include <type_traits>

#include "vipra/concepts/map.hpp"
#include "vipra/concepts/module.hpp"
#include "vipra/concepts/pedset.hpp"
#include "vipra/modules.hpp"

namespace VIPRA::Concepts {

template <typename goals_t>
concept can_get_goals = requires(goals_t goals) {
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
concept GoalsModule = is_module<goals_t, VIPRA::Modules::Type::GOALS> && can_initialize_goals<goals_t> &&
    can_get_goals<goals_t>;

class DummyGoals {
  // NOLINTBEGIN
  VIPRA_MODULE_TYPE(GOALS)
 public:
  template <typename pedset_t, typename map_t>
  void initialize(const pedset_t& /*unused*/, const map_t& /*unused*/) {}

  template <typename params_t>
  static void register_params() {}

  auto current_goals() -> const VIPRA::f3dVec& { return VIPRA::f3dVec{}; }
  auto end_goals() -> const VIPRA::f3dVec& { return VIPRA::f3dVec{}; }
  auto current_goal(VIPRA::idx) -> VIPRA::f3d { return VIPRA::f3d{}; }
  auto end_goal(VIPRA::idx) -> VIPRA::f3d { return VIPRA::f3d{}; }
  auto is_goal_met(VIPRA::idx) -> bool { return false; }
  auto is_sim_goal_met() -> bool { return false; }

  void setup(auto& /*unused*/) {}
  // NOLINTEND
};

CHECK_MODULE(GoalsModule, Concepts::DummyGoals);
}  // namespace VIPRA::Concepts
