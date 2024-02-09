#ifndef VIPRA_HUMAN_BEHAVIOR_HPP
#define VIPRA_HUMAN_BEHAVIOR_HPP

#include "definitions/behavior_context.hpp"
#include "definitions/dsl_types.hpp"
#include "vipra/concepts/pedset.hpp"
#include "vipra/geometry/f3d.hpp"

#include "actions/action.hpp"
#include "events/event.hpp"
#include "goals/goals.hpp"
#include "locations/location.hpp"
#include "obstacle_set/obstacle_set.hpp"
#include "pedestrian_set/pedestrian_set.hpp"
#include "randomization/random.hpp"
#include "selectors/selector.hpp"

namespace BHVR {
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

  void initialize(VIPRA::Concepts::PedsetModule auto const&, const VIPRA::ObstacleSet&, VIPRA::Goals&);
  void timestep(VIPRA::PedestrianSet&, VIPRA::ObstacleSet&, VIPRA::Goals&, VIPRA::State&, VIPRA::delta_t);

  void set_all_ped_types(Ptype);
  void add_sub_selector(SubSelector const&);
  void add_action(typeUID, Action const&);
  auto add_event(Event const&) -> VIPRA::idx;
  auto add_location(Location const&) -> VIPRA::idx;

  [[nodiscard]] auto event_count() const -> VIPRA::size;
  [[nodiscard]] auto selector_count() const -> VIPRA::size;
  [[nodiscard]] auto action_count() const -> VIPRA::size;

  void set_seed(BHVR::seed);

 private:
  BHVR::seed _seedNum{};

  std::string     _name;
  BehaviorContext _context;

  Selector                         _selector;
  std::vector<Event>               _events;
  std::vector<Location*>           _locations;
  std::vector<std::vector<Action>> _actions;
  std::vector<bool>                _conditionMet;
  std::vector<Target>              _targets;

  void evaluate_events(VIPRA::PedestrianSet&, VIPRA::ObstacleSet&, VIPRA::Goals&, VIPRA::delta_t);
  void apply_actions(VIPRA::PedestrianSet&, VIPRA::ObstacleSet&, VIPRA::Goals&, VIPRA::State&,
                     VIPRA::delta_t);
};
}  // namespace BHVR

#endif
