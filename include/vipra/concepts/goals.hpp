#pragma once

#include <concepts>
#include <type_traits>

#include "vipra/concepts/map.hpp"
#include "vipra/concepts/module.hpp"
#include "vipra/concepts/pedset.hpp"
#include "vipra/modules.hpp"

namespace VIPRA::Concepts {

template <typename goals_t>
concept Goals_Initialization = requires(goals_t goals, const DummyPedSet& pedset, const DummyMap& map) {
  {goals.initialize(pedset, map)};
};

template <typename goals_t>
concept GoalsModule = Concepts::Module<goals_t, VIPRA::Modules::Type::GOALS> && Goals_Initialization<goals_t>;

class DummyGoals {
  VIPRA_MODULE_TYPE(GOALS)
 public:
  template <typename pedset_t, typename map_t>
  void initialize(const pedset_t& /*unused*/, const map_t& /*unused*/) {}

  template <typename params_t>
  static void register_params() {}

  void setup(auto& /*unused*/) {}
};

CHECK_MODULE(GoalsModule, Concepts::DummyGoals);
}  // namespace VIPRA::Concepts
