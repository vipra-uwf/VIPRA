
#include "vipra/special_modules/behavior_model.hpp"

namespace VIPRA::Behaviors {

void BehaviorModel::initialize(VIPRA::Modules::Pedestrians const& pedset,
                               VIPRA::Modules::Goals const&       goals,
                               VIPRA::Modules::Map const& map, size_t seed)
{
  _pedCount = pedset.num_pedestrians();
}

void BehaviorModel::update(VIPRA::Modules::Pedestrians const& pedset,
                           VIPRA::Modules::Goals const&       goals,
                           VIPRA::Modules::Map const& map, VIPRA::State& state,
                           VIPRA::delta_t deltaT)
{
  // update time
  _environment.add_time(deltaT);
  const BADL::ProgramInterface interface{pedset, goals, map, &state};

  // apply each of the stimuli to each pedestrian
  std::apply(
      [&](auto const&... stimuli) { (..., apply_stimuli(stimuli, map)); },
      _environment.all_sources());

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

auto BehaviorModel::check_sound(BADL::Agent const& agent, Sound const& sound,
                                VIPRA::Modules::Map const& map) noexcept -> bool
{
  // TODO(rolland): this only checks line of sight, may want to allow sound to go through walls, around corners, etc
  // auto const& pos = agent.get_belief<VIPRA::Position>();
  // return (sound.position.distance_to_sqrd(pos.position) <
  //         SOUND_MAX_DIST_SQRD_M) &&
  //        ! (map.ray_hit(agent.get_belief<Position>().position,
  //                       sound.position) ==
  //           std::numeric_limits<VIPRA::f_pnt>::max());
  return false;
}

auto BehaviorModel::check_sight(BADL::Agent const& agent, Sight const& sight,
                                VIPRA::Modules::Map const& map) noexcept -> bool
{
  return false;
  // return ! (
  // map.ray_hit(agent.get_belief<Position>().position, sight.position) ==
  // std::numeric_limits<VIPRA::f_pnt>::max());
}

}  // namespace VIPRA::Behaviors