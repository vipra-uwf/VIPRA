#pragma once

#include "vipra/concepts/goals.hpp"
#include "vipra/concepts/map.hpp"
#include "vipra/concepts/pedset.hpp"

#include "vipra/geometry/f3d.hpp"

#include "vipra/vipra_behaviors/actions/action.hpp"
#include "vipra/vipra_behaviors/conditions/sub_condition.hpp"
#include "vipra/vipra_behaviors/definitions/behavior_context.hpp"
#include "vipra/vipra_behaviors/definitions/dsl_types.hpp"
#include "vipra/vipra_behaviors/events/event.hpp"
#include "vipra/vipra_behaviors/locations/location.hpp"
#include "vipra/vipra_behaviors/selectors/selector.hpp"
#include "vipra/vipra_behaviors/selectors/subselector.hpp"
#include "vipra/vipra_behaviors/targets/target.hpp"
#include "vipra/vipra_behaviors/targets/target_modifier.hpp"

namespace VIPRA::Behaviors {
/**
 * Describes a specific human behavior. Implementations can either define the behavior directly in C++ or use a DSL.
 */
template <Concepts::PedsetModule pedset_t, Concepts::MapModule map_t, Concepts::GoalsModule goals_t>
class HumanBehavior {
  DEFAULT_CONSTRUCTIBLE(HumanBehavior)
  COPYABLE(HumanBehavior)
  MOVEABLE(HumanBehavior)

 public:
  using pack_t = Simpack<pedset_t, map_t, goals_t>;
  using context_t = BehaviorContext<pedset_t, map_t, goals_t>;
  using cond_t = Condition<SubCondition<pack_t>>;

  using targetfunc_t = TargetFunc<pack_t>;
  using targetmodifier_t = TargetModifier<Modifier<pack_t>>;
  using targetselector_t = TargetSelector<TargetFunc<pack_t>>;
  using atom_t = Atom<pack_t>;
  using action_t = Action<atom_t, cond_t, targetselector_t>;

  using selectorfunc_t = SelectorFunc<pack_t>;
  using subselector_t = SubSelector<SelectorFunc<pack_t>, cond_t>;
  using selector_t = Selector<subselector_t, context_t>;

  using event_t = Event<cond_t>;

  explicit HumanBehavior(std::string behaviorName) : _name(std::move(behaviorName)), _context() {}

  void initialize(pedset_t const& pedset, map_t const& map, goals_t& goals);
  void timestep(pedset_t& pedset, map_t& map, goals_t& goals, VIPRA::State& state, VIPRA::delta_t deltaT);

  void set_all_ped_types(Ptype types) {
    _selector.set_all_types(types);
    _actions.resize(types.type_count() + 1);
  }
  void add_sub_selector(auto const& subSelector) { _selector.add_sub_selector(subSelector); }
  void add_action(typeUID type, auto const& action) { _actions[type].emplace_back(action); }
  auto add_event(event_t const& evnt) -> VIPRA::idx {
    _context.events.push_back(evnt);
    return _context.events.size() - 1;
  }
  auto add_location(Location const& loc) -> VIPRA::idx {
    _context.locations.emplace_back(loc);
    return _context.locations.size() - 1;
  }

  [[nodiscard]] auto get_name() const noexcept -> std::string const& { return _name; }
  [[nodiscard]] auto event_count() const noexcept -> VIPRA::size { return _context.events.size(); }
  [[nodiscard]] auto location_count() const noexcept -> VIPRA::size { return _context.locations.size(); }
  [[nodiscard]] auto selector_count() noexcept -> VIPRA::size { return _selector.selector_count(); }
  [[nodiscard]] auto action_count() const noexcept -> VIPRA::size {
    return std::accumulate(_actions.begin(), _actions.end(), 0,
                           [](VIPRA::size sum, auto const& group) { return sum + group.size(); });
  }

  void set_seed(VIPRA::seed seed) { _seedNum = seed; }

 private:
  VIPRA::seed _seedNum{};

  std::string _name;
  context_t   _context;

  Selector<subselector_t, context_t> _selector;
  std::vector<std::vector<action_t>> _actions;
  std::vector<bool>                  _conditionMet;
  std::vector<Target>                _targets;

  void evaluate_events(pedset_t& pedset, map_t& map, goals_t& goals, VIPRA::delta_t deltaT);
  void apply_actions(pedset_t& pedset, map_t& map, goals_t& goals, VIPRA::State& state,
                     VIPRA::delta_t deltaT);
};

/**
 * @brief initializes behavior selector
 *
 * @param pedSet : pedestrian set object
 * @param obsSet : obstacle set object
 * @param goals : goals object
 */
template <Concepts::PedsetModule pedset_t, Concepts::MapModule map_t, Concepts::GoalsModule goals_t>
void HumanBehavior<pedset_t, map_t, goals_t>::initialize(pedset_t const& pedset, map_t const& map,
                                                         goals_t& goals) {
  VIPRA::State dummyState;

  _context.engine = VIPRA::Random::Engine{_seedNum};
  _context.pedStates = std::vector<Behaviors::stateUID>(pedset.num_pedestrians());
  _context.types = std::vector<Behaviors::typeUID>(pedset.num_pedestrians());

  _conditionMet.resize(pedset.num_pedestrians(), false);
  _targets.resize(pedset.num_pedestrians());
  for ( VIPRA::idx i = 0; i < pedset.num_pedestrians(); ++i ) {
    _targets[i] = Target{TargetType::PEDESTRIAN, i};
  }

  Simpack<pedset_t, map_t, goals_t> pack{pedset, map, goals, _selector.get_groups(), _context, dummyState, 0};
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
 * @tparam pedset_t 
 * @tparam map_t 
 * @tparam goals_t 
 * @param pedset 
 * @param map 
 * @param goals 
 * @param state 
 * @param deltaT 
 */
template <Concepts::PedsetModule pedset_t, Concepts::MapModule map_t, Concepts::GoalsModule goals_t>
void HumanBehavior<pedset_t, map_t, goals_t>::timestep(pedset_t& pedset, map_t& map, goals_t& goals,
                                                       VIPRA::State& state, VIPRA::delta_t deltaT) {
  evaluate_events(pedset, map, goals, deltaT);
  apply_actions(pedset, map, goals, state, deltaT);
  _context.elapsedTime += deltaT;
}

template <Concepts::PedsetModule pedset_t, Concepts::MapModule map_t, Concepts::GoalsModule goals_t>
void HumanBehavior<pedset_t, map_t, goals_t>::evaluate_events(pedset_t& pedset, map_t& map, goals_t& goals,
                                                              VIPRA::delta_t deltaT) {
  VIPRA::State dummyState;
  for ( auto& event : _context.events ) {
    event.evaluate(HumanBehavior<pedset_t, map_t, goals_t>::pack_t{pedset, map, goals, _selector.get_groups(),
                                                                   _context, dummyState, deltaT});
  }
}

template <Concepts::PedsetModule pedset_t, Concepts::MapModule map_t, Concepts::GoalsModule goals_t>
void HumanBehavior<pedset_t, map_t, goals_t>::apply_actions(pedset_t& pedset, map_t& map, goals_t& goals,
                                                            VIPRA::State& state, VIPRA::delta_t deltaT) {
  GroupsContainer&                                groups = _selector.get_groups();
  const VIPRA::size                               groupCnt = groups.size();
  HumanBehavior<pedset_t, map_t, goals_t>::pack_t pack{pedset,   map,   goals, _selector.get_groups(),
                                                       _context, state, deltaT};

  // check if any of the pedestrians should have an action applied
  for ( VIPRA::idx i = 0; i < groupCnt; ++i ) {
    auto& pedestrians = groups[i];

    // remove any pedestrians from the group if they have left the simulation
    pedestrians.erase(std::remove_if(pedestrians.begin(), pedestrians.end(),
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
        action.condition()->evaluate(pack, pedestrians, _conditionMet, _targets, action.duration());
        action.perform_action(pack, pedestrians, _conditionMet, _targets);
      } else {
        // unconditional
        action.perform_action(pack, pedestrians, _targets);
      }
    });
  }
}

}  // namespace VIPRA::Behaviors
