
#pragma once

#include "vipra/vipra_behaviors/senses/sight.hpp"
#include "vipra/vipra_behaviors/senses/sound.hpp"

#include "vipra/modules/map.hpp"
#include "vipra/modules/pedestrians.hpp"
#include "vipra/types/time.hpp"

#include "badl/agent.hpp"
#include "badl/environment/environment.hpp"

namespace VIPRA::Behaviors {
class BehaviorModel : public VIPRA::Modules::Module<BehaviorModel> {
 public:
  VIPRA_MODULE_NAME("main");
  VIPRA_MODULE_TYPE(Behavior_model);

  // NOLINTNEXTLINE(misc-unused-parameters)
  VIPRA_REGISTER_PARAMS(VIPRA_PARAM("behaviors_dir", _behaviorsDir),
                        VIPRA_PARAM("behaviors", _behaviorNames))

  void initialize(VIPRA::Modules::Pedestrians const& pedset,
                  VIPRA::Modules::Goals const&       goals,
                  VIPRA::Modules::Map const& map, size_t seed);

  void update(VIPRA::Modules::Pedestrians const& pedset,
              VIPRA::Modules::Goals const&       goals,
              VIPRA::Modules::Map const& map, VIPRA::State& state,
              VIPRA::delta_t deltaT);

 private:
  static constexpr double SOUND_MAX_DIST_SQRD_M = 25.0;

  std::string              _behaviorsDir;
  std::vector<std::string> _behaviorNames;
  size_t                   _pedCount{};

  BADL::Environment<Sound, Sight> _environment;
  std::vector<BADL::Agent>        _agents;

  template <typename stimulus_t>
  void apply_stimuli(std::vector<stimulus_t> const& sources,
                     VIPRA::Modules::Map const&     map);
  template <typename stimulus_t>
  void apply_stimulus(BADL::Agent& agent, stimulus_t const& stimulus,
                      VIPRA::Modules::Map const& map);

  template <typename stimulus_t>
  static auto can_sense(stimulus_t const& stimulus) -> bool;

  [[nodiscard]] static auto check_sound(
      BADL::Agent const& agent, Sound const& sound,
      VIPRA::Modules::Map const& map) noexcept -> bool;
  [[nodiscard]] static auto check_sight(
      BADL::Agent const& agent, Sight const& sight,
      VIPRA::Modules::Map const& map) noexcept -> bool;
};
}  // namespace VIPRA::Behaviors