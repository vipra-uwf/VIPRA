
#include "vipra/special_modules/behavior_model.hpp"
#include "badl/agent.hpp"
#include "badl/components/behaviors/dsl_behavior.hpp"

namespace VIPRA::Behaviors {

void BehaviorModel::initialize(VIPRA::Modules::Pedestrians const& pedset,
                               VIPRA::Modules::Goals const&       goals,
                               VIPRA::Modules::Map const& map, size_t seed)
{
  _pedCount = pedset.num_pedestrians();

  // TODO(rolland): select pedestrians for behaviors and assign their ids to agents
  _agents = std::vector<BADL::Agent>(1);

  if ( ! _agents.empty() ) _builder.initialize(_agents[0]);

  std::for_each(
      _behaviorNames.begin(), _behaviorNames.end(), [&](auto const& name) {
        auto const filePath = _behaviorsDir + '/' + (name + ".bhvr");

        auto behavior =
            _builder.build_behavior(std::filesystem::path(filePath));

        _agents[0].get_component<BADL::Behaviors>().add_behavior(behavior);
      });

  _environment.add_source(
      VIPRA::Sound{VIPRA::Sound::id("test"), pedset.ped_coords(0)});
}

void BehaviorModel::update(VIPRA::Modules::Pedestrians const& pedset,
                           VIPRA::Modules::Goals const&       goals,
                           VIPRA::Modules::Map const& map, VIPRA::State& state,
                           VIPRA::delta_t deltaT)
{
  // update time
  _environment.add_time(deltaT);
  const BADL::ProgramInterface interface{pedset, goals, map, &state};

  // update the pedestrian's position then decide what to do, do it
  for ( auto& agent : _agents ) {
    // agent.update_belief<Position>(
    //     pedset.ped_coords(agent.get_belief<VIPRA::Identity>()));
    agent.decide(interface, _environment, _environment.get_current_time());
    agent.take_action(interface, _environment, _environment.get_current_time());
  }
}

template <typename stimulus_t>
void BehaviorModel::apply_stimuli(std::vector<stimulus_t> const& sources,
                                  VIPRA::Modules::Map const&     map)
{
  for ( auto& agent : _agents ) {
    for ( auto const& stimulus : sources ) {
      apply_stimulus(agent, stimulus, map);
    }
  }
}

template <typename stimulus_t>
void BehaviorModel::apply_stimulus(BADL::Agent&               agent,
                                   stimulus_t const&          stimulus,
                                   VIPRA::Modules::Map const& map)
{
  if constexpr ( std::is_same_v<Sound, stimulus_t> ) {
    bool canPerceive = check_sound(agent, stimulus, map);
    agent.apply_stimulus(stimulus, _environment.get_current_time());
  }
  else if constexpr ( std::is_same_v<Sight, stimulus_t> ) {
    bool canPerceive = check_sight(agent, stimulus, map);
    agent.apply_stimulus(stimulus, _environment.get_current_time());
  }
  else {
    static_assert(false, "Incorrect Stimulus Type");
  }
}

}  // namespace VIPRA::Behaviors