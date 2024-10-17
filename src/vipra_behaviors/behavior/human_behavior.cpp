#include "vipra/vipra_behaviors/behavior/human_behavior.hpp"

namespace VIPRA::Behaviors {

/**
 * @brief initializes behavior selector
 *
 * @param pedSet : pedestrian set object
 * @param map : map object
 * @param goals : goals object
 */

void HumanBehavior::initialize(Modules::Pedestrians const& pedset,
                               Modules::Map const& map, Modules::Goals& goals)
{
  VIPRA::State dummyState;

  _context.engine = VIPRA::Random::Engine{_seedNum};
  _context.pedStates =
      std::vector<Behaviors::stateUID>(pedset.num_pedestrians());
  _context.types = std::vector<Behaviors::typeUID>(pedset.num_pedestrians());

  _conditionMet.resize(pedset.num_pedestrians(), false);
  _targets.resize(pedset.num_pedestrians());
  for ( VIPRA::idx i = 0; i < pedset.num_pedestrians(); ++i ) {
    _targets[i] = Target{TargetType::PEDESTRIAN, i};
  }

  Simpack pack{pedset,   map,        goals, _selector.get_groups(),
               _context, dummyState, 0};
  _selector.initialize(_name, pack);

  for ( auto& actionGroup : _actions ) {
    for ( auto& action : actionGroup ) {
      action.initialize(pack);
    }
  }
}

/**
 * @brief Evaluates behavior events, and performs actions of pedestrians
 * 
 * @tparam Modules::Pedestrians 
 * @tparam Modules::Map 
 * @tparam Modules::Goals 
 * @param pedset 
 * @param map 
 * @param goals 
 * @param state 
 * @param deltaT 
 */

void HumanBehavior::timestep(Modules::Pedestrians& pedset, Modules::Map& map,
                             Modules::Goals& goals, VIPRA::State& state,
                             VIPRA::delta_t deltaT)
{
  evaluate_events(pedset, map, goals, deltaT);
  apply_actions(pedset, map, goals, state, deltaT);
  _context.elapsedTime += deltaT;
}

void HumanBehavior::evaluate_events(Modules::Pedestrians& pedset,
                                    Modules::Map& map, Modules::Goals& goals,
                                    VIPRA::delta_t deltaT)
{
  VIPRA::State dummyState;
  for ( auto& event : _context.events ) {
    event.evaluate(Simpack{pedset, map, goals, _selector.get_groups(), _context,
                           dummyState, deltaT});
  }
}

void HumanBehavior::apply_actions(Modules::Pedestrians& pedset,
                                  Modules::Map& map, Modules::Goals& goals,
                                  VIPRA::State& state, VIPRA::delta_t deltaT)
{
  GroupsContainer&  groups = _selector.get_groups();
  const VIPRA::size groupCnt = groups.size();
  Simpack           pack{pedset,   map,   goals, _selector.get_groups(),
               _context, state, deltaT};

  // check if any of the pedestrians should have an action applied
  for ( VIPRA::idx i = 0; i < groupCnt; ++i ) {
    auto& pedestrians = groups[i];

    // for each of the group's actions, check if it should apply and apply it
    std::for_each(_actions[i].begin(), _actions[i].end(), [&](auto& action) {
      if ( action.has_target() ) {
        action.targets()->get_targets(pack, pedestrians, _targets);
        // TODO(rolland): this needs to go back to self, if there is an action that changes it
      }

      if ( action.has_condition() ) {
        // check condition and apply
        action.condition()->evaluate(pack, pedestrians, _conditionMet, _targets,
                                     action.duration());
        action.perform_action(pack, pedestrians, _conditionMet, _targets);
      }
      else {
        // unconditional
        action.perform_action(pack, pedestrians, _targets);
      }
    });

    // remove any pedestrians from the group if they have left the simulation
    pedestrians.erase(
        std::remove_if(pedestrians.begin(), pedestrians.end(),
                       [&](VIPRA::idx ped) { return goals.is_goal_met(ped); }),
        pedestrians.end());
  }
}
}  // namespace VIPRA::Behaviors