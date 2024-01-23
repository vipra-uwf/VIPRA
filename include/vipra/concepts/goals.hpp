#pragma once

#include <concepts>
#include <type_traits>

#include "vipra/concepts/module.hpp"
#include "vipra/modules.hpp"

namespace VIPRA::Concepts {
template <typename goals_t>
concept GoalsModule = Concepts::Module<goals_t, VIPRA::Modules::Type::GOALS>;

class DummyGoals {
  VIPRA_MODULE_TYPE(GOALS)
 public:
  template <typename params_t>
  static void register_params() {}

  void setup(auto& /*unused*/) {}
};

CHECK_MODULE(GoalsModule, Concepts::DummyGoals);
}  // namespace VIPRA::Concepts
