#pragma once

#include "vipra/macros/all_macros.hpp"

#include "vipra/macros/parameters.hpp"
#include "vipra/modules/module.hpp"
#include "vipra/types/seed.hpp"
#include "vipra/vipra_behaviors/attributes/attributes.hpp"
#include "vipra/vipra_behaviors/behavior/human_behavior.hpp"
#include "vipra/vipra_behaviors/builder/behavior_builder.hpp"

namespace VIPRA {
/**
 * @brief BehaviorModel is a special module that is responsible for loading and managing all behaviors
 * 
 */
template <typename pedset_t, typename map_t, typename goals_t>
class BehaviorModel : public VIPRA::Modules::Module<BehaviorModel<pedset_t, map_t, goals_t>> {
 public:
  VIPRA_MODULE_NAME("main");
  VIPRA_MODULE_TYPE(Behavior_model);

  // NOLINTNEXTLINE(misc-unused-parameters)
  VIPRA_REGISTER_PARAMS(VIPRA_PARAM("behaviors_dir", _behaviorsDir), VIPRA_PARAM("behaviors", _behaviorNames))

  void initialize(pedset_t& pedset, map_t& map, goals_t& goals, VIPRA::seed seed)
  {
    // TODO(rolland): figure out why it errors when duplicating behaviors

    _behaviors.clear();
    Behaviors::AttributeHandling::cleanup();

    load_behaviors(seed);
    for ( auto& behavior : _behaviors ) {
      behavior.initialize(pedset, map, goals);
    }
  }

  void timestep(pedset_t& pedset, map_t& map, goals_t& goals, VIPRA::State& state, VIPRA::delta_t deltaT)
  {
    for ( auto& behavior : _behaviors ) {
      behavior.timestep(pedset, map, goals, state, deltaT);
    }
  }

 private:
  std::string                                                     _behaviorsDir;
  std::vector<std::string>                                        _behaviorNames;
  std::vector<Behaviors::HumanBehavior<pedset_t, map_t, goals_t>> _behaviors;

  void load_behaviors(VIPRA::seed seed)
  {
    Behaviors::BehaviorBuilder<pedset_t, map_t, goals_t> builder;
    std::transform(_behaviorNames.begin(), _behaviorNames.end(), std::back_inserter(_behaviors),
                   [&](auto const& name) {
                     auto const filePath = _behaviorsDir + '/' + (name + ".bhvr");
                     return builder.build(name, filePath, seed);
                   });
  }

 public:
  BehaviorModel() = default;
  BehaviorModel(BehaviorModel const&) = default;
  auto operator=(BehaviorModel const&) -> BehaviorModel& = default;
  BehaviorModel(BehaviorModel&&) noexcept = default;
  auto operator=(BehaviorModel&&) noexcept -> BehaviorModel& = default;
  ~BehaviorModel() { Behaviors::AttributeHandling::cleanup(); };
};
}  // namespace VIPRA