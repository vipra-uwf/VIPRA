#pragma once

#include "definitions/sim_pack.hpp"

#include "vipra/modules/goals.hpp"
#include "vipra/modules/map.hpp"
#include "vipra/modules/pedestrians.hpp"
#include "vipra/vipra_behaviors/actions/action.hpp"
#include "vipra/vipra_behaviors/definitions/behavior_context.hpp"
#include "vipra/vipra_behaviors/definitions/dsl_types.hpp"
#include "vipra/vipra_behaviors/events/event.hpp"
#include "vipra/vipra_behaviors/locations/location.hpp"
#include "vipra/vipra_behaviors/selectors/selector.hpp"
#include "vipra/vipra_behaviors/targets/target.hpp"

namespace VIPRA::Behaviors {
/**
 * Describes a specific human behavior. Implementations can either define the behavior directly in C++ or use a DSL.
 */
class HumanBehavior {
  DEFAULT_CONSTRUCTIBLE(HumanBehavior)
  COPYABLE(HumanBehavior)
  MOVEABLE(HumanBehavior)

 public:
  explicit HumanBehavior(std::string behaviorName)
      : _name(std::move(behaviorName)), _context()
  {
  }

  void initialize(Modules::Pedestrians const& pedset, Modules::Map const& map,
                  Modules::Goals& goals);
  void timestep(Modules::Pedestrians& pedset, Modules::Map& map,
                Modules::Goals& goals, VIPRA::State& state,
                VIPRA::delta_t deltaT);

  void set_all_ped_types(Ptype types)
  {
    _selector.set_all_types(types);
    _actions.resize(types.type_count() + 1);
  }
  void add_sub_selector(auto const& subSelector)
  {
    _selector.add_sub_selector(subSelector);
  }
  void add_action(typeUID type, auto const& action)
  {
    _actions[type].emplace_back(action);
  }
  auto add_event(Event const& evnt) -> VIPRA::idx
  {
    _context.events.push_back(evnt);
    return _context.events.size() - 1;
  }
  auto add_location(Location const& loc) -> VIPRA::idx
  {
    _context.locations.emplace_back(loc);
    return _context.locations.size() - 1;
  }

  [[nodiscard]] auto get_name() const noexcept -> std::string const&
  {
    return _name;
  }
  [[nodiscard]] auto event_count() const noexcept -> VIPRA::size
  {
    return _context.events.size();
  }
  [[nodiscard]] auto location_count() const noexcept -> VIPRA::size
  {
    return _context.locations.size();
  }
  [[nodiscard]] auto selector_count() noexcept -> VIPRA::size
  {
    return _selector.selector_count();
  }
  [[nodiscard]] auto action_count() const noexcept -> VIPRA::size
  {
    return std::accumulate(
        _actions.begin(), _actions.end(), 0,
        [](VIPRA::size sum, auto const& group) { return sum + group.size(); });
  }

  void set_seed(VIPRA::seed seed) { _seedNum = seed; }

 private:
  VIPRA::seed _seedNum{};

  std::string     _name;
  BehaviorContext _context;

  Selector                         _selector;
  std::vector<std::vector<Action>> _actions;
  std::vector<bool>                _conditionMet;
  std::vector<Target>              _targets;

  void evaluate_events(Modules::Pedestrians& pedset, Modules::Map& map,
                       Modules::Goals& goals, VIPRA::delta_t deltaT);
  void apply_actions(Modules::Pedestrians& pedset, Modules::Map& map,
                     Modules::Goals& goals, VIPRA::State& state,
                     VIPRA::delta_t deltaT);
};

/**
 * @brief initializes behavior selector
 *
 * @param pedSet : pedestrian set object
 * @param map : map object
 * @param goals : goals object
 */

inline void HumanBehavior::initialize(Modules::Pedestrians const& pedset,
                                      Modules::Map const&         map,
                                      Modules::Goals&             goals)
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

inline void HumanBehavior::timestep(Modules::Pedestrians& pedset,
                                    Modules::Map& map, Modules::Goals& goals,
                                    VIPRA::State& state, VIPRA::delta_t deltaT)
{
  evaluate_events(pedset, map, goals, deltaT);
  apply_actions(pedset, map, goals, state, deltaT);
  _context.elapsedTime += deltaT;
}

inline void HumanBehavior::evaluate_events(Modules::Pedestrians& pedset,
                                           Modules::Map&         map,
                                           Modules::Goals&       goals,
                                           VIPRA::delta_t        deltaT)
{
  VIPRA::State dummyState;
  for ( auto& event : _context.events ) {
    event.evaluate(Simpack{pedset, map, goals, _selector.get_groups(), _context,
                           dummyState, deltaT});
  }
}

inline void HumanBehavior::apply_actions(Modules::Pedestrians& pedset,
                                         Modules::Map&         map,
                                         Modules::Goals&       goals,
                                         VIPRA::State&         state,
                                         VIPRA::delta_t        deltaT)
{
  GroupsContainer&  groups = _selector.get_groups();
  const VIPRA::size groupCnt = groups.size();
  Simpack           pack{pedset,   map,   goals, _selector.get_groups(),
               _context, state, deltaT};

  // check if any of the pedestrians should have an action applied
  for ( VIPRA::idx i = 0; i < groupCnt; ++i ) {
    auto& pedestrians = groups[i];

    // remove any pedestrians from the group if they have left the simulation
    pedestrians.erase(
        std::remove_if(pedestrians.begin(), pedestrians.end(),
                       [&](VIPRA::idx ped) { return goals.is_goal_met(ped); }),
        pedestrians.end());

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
  }
}

}  // namespace VIPRA::Behaviors
