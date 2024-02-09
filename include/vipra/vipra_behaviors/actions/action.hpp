#ifndef ACTION_HPP
#define ACTION_HPP

#include <optional>

#include "definitions/behavior_context.hpp"
#include "definitions/sim_pack.hpp"
#include "vipra/geometry/f3d.hpp"

#include "goals/goals.hpp"
#include "obstacle_set/obstacle_set.hpp"
#include "pedestrian_set/pedestrian_set.hpp"

#include "actions/atom.hpp"
#include "conditions/condition.hpp"

#include "targets/target_selector.hpp"
#include "targets/target_selectors/target_self.hpp"

#include "time/time.hpp"

#include "util/class_types.hpp"
#include "util/timed_latch.hpp"

#include "values/numeric_value.hpp"

namespace BHVR {
class Action {
  DEFAULT_CONSTRUCTIBLE(Action)
  COPYABLE(Action)
  MOVEABLE(Action)

 public:
  void initialize(Simpack pack);

  void perform_action(Simpack pack, VIPRA::idxVec&, std::vector<Target> const&);
  void perform_action(Simpack pack, const VIPRA::idxVec&, std::vector<bool> const&,
                      std::vector<Target> const&);

  void add_condition(Condition const&);
  void add_atom(Atom const&);
  void add_duration(const BHVR::NumericValue&);
  void add_target(TargetSelector&&);

  [[nodiscard]] auto has_condition() -> bool { return _condition.has_value(); }
  [[nodiscard]] auto condition() -> std::optional<Condition>& { return _condition; }

  [[nodiscard]] auto has_duration() -> bool { return _duration.has_value(); }
  [[nodiscard]] auto duration() -> std::optional<TimedLatchCollection>& { return _duration; }

  [[nodiscard]] auto has_target() -> bool { return _targets.has_value(); }
  [[nodiscard]] auto targets() -> std::optional<TargetSelector>& { return _targets; }

 private:
  std::vector<Atom>                   _atoms;
  std::optional<Condition>            _condition;
  std::optional<TimedLatchCollection> _duration;
  std::optional<TargetSelector>       _targets;
};

}  // namespace BHVR

#endif