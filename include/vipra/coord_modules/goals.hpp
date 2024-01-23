#pragma once

#include <tuple>

#include "vipra/concepts/goals.hpp"

namespace VIPRA {
template <Concepts::GoalsModule... goals_ts>
class Goals {
  VIPRA_MODULE_TYPE(GOALS);

 public:
  constexpr explicit Goals(goals_ts... goals) : _goals(std::make_tuple(goals...)) {}

  /**
   * @brief Calls setup on all goals modules
   * 
   * @param params 
   */
  void setup(const auto& params) {
    std::apply([&params](auto&&... goals) { (goals.setup(params), ...); }, _goals);
  }

  /**
   * @brief Calls register_params on all goals modules
   * 
   * @tparam params_t 
   */
  template <Concepts::ParamModule params_t>
  static void register_params() {
    (goals_ts::template register_params<params_t>(), ...);
  }

 private:
  std::tuple<goals_ts...> _goals;
};

CHECK_MODULE(GoalsModule, Goals<Concepts::DummyGoals>);
}  // namespace VIPRA