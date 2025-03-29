#pragma once

#include "vipra/macros/module.hpp"

#include "vipra/macros/parameters.hpp"
#include "vipra/modules/goals.hpp"
#include "vipra/modules/module.hpp"
#include "vipra/modules/pedestrians.hpp"

#include "vipra/types/seed.hpp"
#include "vipra/util/clock.hpp"
#include "vipra/util/timing.hpp"
#include "vipra/vipra_behaviors/attributes/attributes.hpp"
#include "vipra/vipra_behaviors/behavior/human_behavior.hpp"
#include "vipra/vipra_behaviors/builder/behavior_builder.hpp"

namespace VIPRA {
/**
 * @brief BehaviorModel is a special module that is responsible for loading and managing all behaviors
 * 
 */
class BehaviorModel : public VIPRA::Modules::Module<BehaviorModel> {
 public:
  VIPRA_MODULE_NAME("main");
  VIPRA_MODULE_TYPE(Behavior_model);

  // NOLINTNEXTLINE(misc-unused-parameters)
  VIPRA_REGISTER_PARAMS(VIPRA_PARAM("behaviors_dir", _behaviorsDir),
                        VIPRA_PARAM("behaviors", _behaviorNames))

  void initialize(Modules::Pedestrians& pedset, Modules::Map& map, Modules::Goals& goals,
                  VIPRA::seed seed)
  {
    // TODO(rolland): figure out why it errors when duplicating behaviors

    _behaviors.clear();
    Behaviors::AttributeHandling::cleanup();

    build_behaviors(map, seed);
    for ( auto& behavior : _behaviors ) {
      behavior.initialize(pedset, map, goals);
    }

    behaviorTimings.start_new();
    behaviorTimings.pause();
  }

  void timestep(Modules::Pedestrians& pedset, Modules::Map& map, Modules::Goals& goals,
                VIPRA::State& state, VIPRA::delta_t deltaT)
  {
    behaviorTimings.resume();
    for ( auto& behavior : _behaviors ) {
      behavior.timestep(pedset, map, goals, state, deltaT);
    }
    behaviorTimings.pause();
  }

 private:
  std::string                           _behaviorsDir;
  std::vector<std::string>              _behaviorNames;
  std::vector<Behaviors::HumanBehavior> _behaviors;

  static Util::Timings behaviorTimings;

  void build_behaviors(Modules::Map const& map, VIPRA::seed seed)
  {
    Behaviors::BehaviorBuilder builder;
    std::transform(_behaviorNames.begin(), _behaviorNames.end(),
                   std::back_inserter(_behaviors), [&](auto const& name) {
                     auto const filePath = _behaviorsDir + '/' + (name + ".bhvr");
                     return builder.build(name, filePath, map, seed);
                   });
  }

 public:
  BehaviorModel() = default;
  BehaviorModel(BehaviorModel const&) = default;
  auto operator=(BehaviorModel const&) -> BehaviorModel& = default;
  BehaviorModel(BehaviorModel&&) noexcept = default;
  auto operator=(BehaviorModel&&) noexcept -> BehaviorModel& = default;
  ~BehaviorModel()
  {
    Behaviors::AttributeHandling::cleanup();
    behaviorTimings.stop();
    behaviorTimings.output_timings();
  };
};
}  // namespace VIPRA