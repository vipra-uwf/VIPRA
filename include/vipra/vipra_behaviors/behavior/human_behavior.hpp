#pragma once

#include "vipra/modules/goals.hpp"
#include "vipra/modules/map.hpp"
#include "vipra/modules/pedestrians.hpp"
#include "vipra/vipra_behaviors/actions/action.hpp"
#include "vipra/vipra_behaviors/definitions/behavior_context.hpp"
#include "vipra/vipra_behaviors/definitions/sim_pack.hpp"
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

}  // namespace VIPRA::Behaviors
