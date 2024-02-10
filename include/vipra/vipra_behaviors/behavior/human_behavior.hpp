#pragma once

#include "vipra/concepts/goals.hpp"
#include "vipra/concepts/map.hpp"
#include "vipra/concepts/pedset.hpp"

#include "vipra/geometry/f3d.hpp"

#include "vipra/vipra_behaviors/actions/action.hpp"
#include "vipra/vipra_behaviors/definitions/behavior_context.hpp"
#include "vipra/vipra_behaviors/definitions/dsl_types.hpp"
#include "vipra/vipra_behaviors/events/event.hpp"
#include "vipra/vipra_behaviors/locations/location.hpp"
#include "vipra/vipra_behaviors/selectors/selector.hpp"

namespace VIPRA::Behaviors {
/**
 * Describes a specific human behavior. Implementations can either define the behavior directly in C++ or use a DSL.
 */
class HumanBehavior {
  DEFAULT_CONSTRUCTIBLE(HumanBehavior)
  COPYABLE(HumanBehavior)
  MOVEABLE(HumanBehavior)

 public:
  explicit HumanBehavior(std::string);

  [[nodiscard]] auto get_name() const noexcept -> std::string const&;

  void initialize(Concepts::PedsetModule auto const&, Concepts::MapModule auto const&,
                  Concepts::GoalsModule auto&);
  void timestep(Concepts::PedsetModule&, Concepts::MapModule auto&, Concepts::GoalsModule auto&,
                VIPRA::State&, VIPRA::delta_t);

  void set_all_ped_types(Ptype);
  void add_sub_selector(SubSelector const&);
  void add_action(typeUID, Action const&);
  auto add_event(Event const&) -> VIPRA::idx;
  auto add_location(Location const&) -> VIPRA::idx;

  [[nodiscard]] auto event_count() const -> VIPRA::size;
  [[nodiscard]] auto selector_count() const -> VIPRA::size;
  [[nodiscard]] auto action_count() const -> VIPRA::size;

  void set_seed(Behaviors::seed);

 private:
  Behaviors::seed _seedNum{};

  std::string     _name;
  BehaviorContext _context;

  Selector                         _selector;
  std::vector<Event>               _events;
  std::vector<Location*>           _locations;
  std::vector<std::vector<Action>> _actions;
  std::vector<bool>                _conditionMet;
  std::vector<Target>              _targets;

  void evaluate_events(Concepts::PedsetModule&, Concepts::MapModule auto&, Concepts::GoalsModule auto&,
                       VIPRA::delta_t);
  void apply_actions(Concepts::PedsetModule&, Concepts::MapModule auto&, Concepts::GoalsModule auto&,
                     VIPRA::State&, VIPRA::delta_t);
};
}  // namespace VIPRA::Behaviors
