#pragma once

#include "vipra/concepts/all_concepts.hpp"
#include "vipra/concepts/goals.hpp"
#include "vipra/concepts/pedset.hpp"
#include "vipra/macros/all_macros.hpp"

#include "vipra/macros/parameters.hpp"
#include "vipra/types/seed.hpp"
#include "vipra/vipra_behaviors/behavior/human_behavior.hpp"
#include "vipra/vipra_behaviors/builder/behavior_builder.hpp"

namespace VIPRA {
/**
 * @brief BehaviorModel is a special module that is responsible for loading and managing all behaviors
 * 
 */
template <Concepts::PedsetModule pedset_t, Concepts::MapModule map_t, Concepts::GoalsModule goals_t>
class BehaviorModel {
 public:
  VIPRA_MODULE_NAME("behavior_model");
  VIPRA_MODULE_TYPE(BEHAVIOR_MODEL);

  // NOLINTNEXTLINE(misc-unused-parameters)
  static void register_params(Concepts::ParamModule auto& params) {
    std::cout << "Wot\n";
    VIPRA_REGISTER_PARAM("behaviors_dir", REQUIRED);
    VIPRA_REGISTER_PARAM("behaviors", OPTIONAL);
  }

  // NOLINTNEXTLINE(misc-unused-parameters)
  VIPRA_CONFIG_STEP {
    if (!params.has_required_param(_VIPRA_MODULE_TYPE_, _VIPRA_MODULE_NAME_, "behaviors_dir")) {
      throw std ::runtime_error(
          "Required Parameter: "
          "behaviors_dir"
          " For Module: " +
          to_string(_VIPRA_MODULE_TYPE_) + " Not Provide in Input");
    }
    _behaviorsDir = params.template get_param<std ::remove_cv_t<decltype(_behaviorsDir)>>(
        _VIPRA_MODULE_TYPE_, _VIPRA_MODULE_NAME_, "behaviors_dir");
    ;
    VIPRA_GET_PARAM("behaviors", _behaviorNames);
  }

  void initialize(Concepts::PedsetModule auto const& pedset, Concepts::GoalsModule auto& goals,
                  Concepts::MapModule auto const& map, VIPRA::seed seed) {
    load_behaviors(seed);
    for (auto& behavior : _behaviors) {
      behavior.initialize(pedset, map, goals);
    }
  }

 private:
  std::filesystem::path                                           _behaviorsDir;
  std::vector<std::string>                                        _behaviorNames;
  std::vector<Behaviors::HumanBehavior<pedset_t, map_t, goals_t>> _behaviors;

  void load_behaviors(VIPRA::seed seed) {
    Behaviors::BehaviorBuilder<pedset_t, map_t, goals_t> builder;
    std::transform(_behaviorNames.begin(), _behaviorNames.end(), std::back_inserter(_behaviors),
                   [&](auto const& name) {
                     auto const filePath = _behaviorsDir / (name + ".bhvr");
                     return builder.build(name, filePath, seed);
                   });
  }
};
}  // namespace VIPRA