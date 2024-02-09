#pragma once

#include "vipra/concepts/all_concepts.hpp"
#include "vipra/concepts/goals.hpp"
#include "vipra/concepts/pedset.hpp"
#include "vipra/macros/all_macros.hpp"

#include "vipra/vipra_behaviors/builder/behavior_builder.hpp"

namespace VIPRA {
class BehaviorModel {
 public:
  VIPRA_MODULE_NAME("behavior_model");
  VIPRA_MODULE_TYPE(BEHAVIOR_MODEL);

  // NOLINTNEXTLINE(misc-unused-parameters)
  static void register_params(Concepts::ParamModule auto& params) {
    VIPRA_REGISTER_PARAM("behaviors", OPTIONAL);
  }

  // NOLINTNEXTLINE(misc-unused-parameters)
  VIPRA_CONFIG_STEP { VIPRA_GET_PARAM("behaviors", _behaviors); }

  void initialize(Concepts::PedsetModule auto const& pedset, Concepts::GoalsModule auto const& goals,
                  Concepts::MapModule auto const& map) {}

 private:
  std::vector<std::string> _behaviors;

  void load_behaviors() {
    BHVR::BehaviorBuilder builder;
    spdlog::info("Loading {} Behaviors", behaviors.size());

    if (!behaviors.is_array())
      BehaviorModelException::error("Configuration File Behaviors not in proper format");

    for (const auto& value : behaviors.items()) {
      const auto filePath = BEHAVIORS_DIR / (value.value().get<std::string>() + ".bhvr");
      humanBehaviors.push_back(builder.build(value.value(), filePath, seed));
    }
  }
};
}  // namespace VIPRA