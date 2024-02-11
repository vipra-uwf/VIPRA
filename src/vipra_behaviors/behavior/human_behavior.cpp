
#include "behavior/human_behavior.hpp"

#include <algorithm>
#include <numeric>
#include <randomization/random.hpp>
#include <utility>

#include "definitions/dsl_types.hpp"
#include "definitions/pedestrian_types.hpp"
#include "definitions/sim_pack.hpp"
#include "definitions/state.hpp"
#include "selectors/selector.hpp"
#include "targets/target.hpp"

namespace VIPRA::Behaviors {

/**
 * @brief Returns the name of the behavior
 * 
 * @return const std::string& 
 */
auto HumanBehavior::get_name() const noexcept -> std::string const& { return _name; }

/**
 * @brief Adds an action to the selectors with type
 * 
 * @param action : action to add
 */
void HumanBehavior::add_action(typeUID type, Action const& action) {
  const VIPRA::idx ndx = GroupsContainer::index(type);
  _actions[ndx].emplace_back(action);
}

/**
 * @brief Adds the selector to the behavior
 * 
 * @param selector
 */
void HumanBehavior::add_sub_selector(SubSelector const& subSelector) {
  _selector.add_sub_selector(subSelector);
}

/**
 * @brief Sets the Behaviors selectors allTypes
 * 
 */
void HumanBehavior::set_all_ped_types(Ptype types) {
  _selector.set_all_types(types);
  _actions.resize(types.type_count() + 1);
}

// /**
//  * @brief initializes behavior selector
//  *
//  * @param pedSet : pedestrian set object
//  * @param obsSet : obstacle set object
//  * @param goals : goals object
//  */
// void HumanBehavior::initialize(const VIPRA::PedestrianSet& pedSet, const VIPRA::ObstacleSet& obsSet,
//                                VIPRA::Goals& goals) {
//   VIPRA::State dummyState;

//   _context.engine = VIPRA::pRNG_Engine{_seedNum};
//   _context.pedStates = std::vector<Behaviors::stateUID>(pedSet.getNumPedestrians());
//   _context.types = std::vector<Behaviors::typeUID>(pedSet.getNumPedestrians());

//   _conditionMet.resize(pedSet.getNumPedestrians(), false);
//   _targets.resize(pedSet.getNumPedestrians());
//   for (VIPRA::idx i = 0; i < pedSet.getNumPedestrians(); ++i) {
//     _targets[i] = Target{TargetType::PEDESTRIAN, i};
//   }

//   Simpack pack{pedSet, obsSet, goals, dummyState, _context, _selector.get_groups(), 0};
//   // spdlog::debug("Initializing {} Selectors, Seed: {}", _selector.selector_count(), _seedNum);
//   _selector.initialize(_name, pack);

//   for (auto& actionGroup : _actions) {
//     for (auto& action : actionGroup) {
//       action.initialize(pack);
//     }
//   }
// }

/**
 * @brief Evaluates behavior events, and performs actions of pedestrians
 * 
 * @param pedSet : pedestrian set object
 * @param obsSet : obstacle set object
 * @param goals : goals object
 * @param state : state object to write results to
 * @param deltaT : simulation timestep size
 */
void HumanBehavior::timestep(VIPRA::PedestrianSet& pedSet, VIPRA::ObstacleSet& obsSet, VIPRA::Goals& goals,
                             VIPRA::State& state, VIPRA::delta_t deltaT) {
  evaluate_events(pedSet, obsSet, goals, deltaT);
  apply_actions(pedSet, obsSet, goals, state, deltaT);
  _context.elapsedTime += deltaT;
}

/**
 * @brief adds an event to the behavior and its index in the behavior context
 * 
 * @param event : event object to add
 * @return VIPRA::idx
 */
auto HumanBehavior::add_event(Event const& event) -> VIPRA::idx {
  _context.events.emplace_back(event);
  return _context.events.size() - 1;
}

/**
 * @brief adds a location to the behavior and its index in the behavior context
 * 
 * @param loc : location object to add
 * @return VIPRA::idx
*/
auto HumanBehavior::add_location(Location const& loc) -> VIPRA::idx {
  _context.locations.emplace_back(loc);
  return _context.locations.size() - 1;
}

/**
 * @brief Returns the number of events the behavior has
 * 
 * @return VIPRA::size 
 */
auto HumanBehavior::event_count() const -> VIPRA::size { return _context.events.size(); }

/**
 * @brief Returns the number of selectors the behavior has
 * 
 * @return VIPRA::size 
 */
auto HumanBehavior::selector_count() const -> VIPRA::size { return _selector.selector_count(); }

/**
 * @brief Returns the number of actions the behavior has
 * 
 * @return VIPRA::size 
 */
auto HumanBehavior::action_count() const -> VIPRA::size {
  VIPRA::size cnt = 0;
  for (auto const& actionGroup : _actions) {
    cnt += actionGroup.size();
  }

  return cnt;
}

/**
 * @brief Sets the seed for the behavior
 * 
 * @param s : randomization seed
 */
void HumanBehavior::set_seed(VIPRA::seed bSeed) {
  _context.engine.reseed(bSeed);
  _seedNum = bSeed;
}

/**
 * @brief Updates each events status, through the events update function
 * 
 * @param pedSet : pedestrian set
 * @param obsSet : obstacle set
 * @param goals : goals module
 * @param deltaT : time between timesteps
 */
void HumanBehavior::evaluate_events(VIPRA::PedestrianSet& pedSet, VIPRA::ObstacleSet& obsSet,
                                    VIPRA::Goals& goals, VIPRA::delta_t deltaT) {
  VIPRA::State dummyState;
  for (auto& event : _context.events) {
    event.evaluate({pedSet, obsSet, goals, dummyState, _context, _selector.get_groups(), deltaT});
  }
}

/**
 * @brief Runs each action in the behavior
 * 
 * @param pedSet : pedestrian set
 * @param obsSet : obstacle set
 * @param goals : goals module
 * @param state : next timestep state from pedestrian model 
 * @param deltaT : timestep size
 */
void HumanBehavior::apply_actions(VIPRA::PedestrianSet& pedSet, VIPRA::ObstacleSet& obsSet,
                                  VIPRA::Goals& goals, VIPRA::State& state, VIPRA::delta_t deltaT) {
  GroupsContainer&  groups = _selector.get_groups();
  const VIPRA::size groupCnt = groups.size();
  Simpack           pack{pedSet, obsSet, goals, state, _context, _selector.get_groups(), deltaT};

  for (VIPRA::idx i = 0; i < groupCnt; ++i) {
    auto& pedestrians = groups[i];
    pedestrians.erase(std::remove_if(pedestrians.begin(), pedestrians.end(),
                                     [&](VIPRA::idx ped) { return goals.isPedestianGoalMet(ped); }),
                      pedestrians.end());

    std::for_each(_actions[i].begin(), _actions[i].end(), [&](auto& action) {
      if (action.has_target()) {
        action.targets()->get_targets(pack, pedestrians, _targets);
        // TODO: this needs to go back to self, if there is an action that changes it
      }

      if (action.has_condition()) {
        action.condition()->evaluate(pack, pedestrians, _conditionMet, _targets, action.duration());
        action.perform_action(pack, pedestrians, _conditionMet, _targets);
      } else {
        action.perform_action(pack, pedestrians, _targets);
      }
    });
  }
}

// ------------------------------------------ CONSTRUCTORS ------------------------------------------------------------------------

HumanBehavior::HumanBehavior(std::string behaviorName) : _name(std::move(behaviorName)), _context() {}

// ------------------------------------------ END CONSTRUCTORS ------------------------------------------------------------------------
}  // namespace VIPRA::Behaviors