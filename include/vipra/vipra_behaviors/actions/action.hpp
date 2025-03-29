#pragma once

#include <optional>

#include "vipra/vipra_behaviors/actions/atom.hpp"
#include "vipra/vipra_behaviors/targets/target.hpp"
#include "vipra/vipra_behaviors/targets/target_selector.hpp"

#include "vipra/vipra_behaviors/conditions/condition.hpp"

#include "vipra/vipra_behaviors/util/class_types.hpp"
#include "vipra/vipra_behaviors/util/timed_latch.hpp"

#include "vipra/vipra_behaviors/values/numeric_value.hpp"

namespace VIPRA::Behaviors {
class Action {
  DEFAULT_CONSTRUCTIBLE(Action)
  COPYABLE(Action)
  MOVEABLE(Action)

 public:
  void initialize(auto pack)
  {
    if ( _condition ) _condition->initialize(pack);
    if ( _duration ) _duration->resize(pack.pedset.num_pedestrians());
  }

  void perform_action(Simpack pack, VIPRA::idxVec& peds,
                      std::vector<Target> const& targets);
  void perform_action(Simpack pack, const VIPRA::idxVec& peds,
                      std::vector<bool> const&   conditionMet,
                      std::vector<Target> const& targets);

  void add_condition(Condition const& cond) { _condition = cond; }
  void add_atom(Atom&& atom) { _atoms.push_back(atom); }
  void add_target(TargetSelector&& target) { _targets = target; }
  void add_duration(Behaviors::NumericValue const& duration)
  {
    _duration = TimedLatchCollection(duration);
  }

  [[nodiscard]] auto has_condition() -> bool { return _condition.has_value(); }
  [[nodiscard]] auto condition() -> std::optional<Condition>& { return _condition; }

  [[nodiscard]] auto has_duration() -> bool { return _duration.has_value(); }
  [[nodiscard]] auto duration() -> std::optional<TimedLatchCollection>&
  {
    return _duration;
  }

  [[nodiscard]] auto has_target() -> bool { return _targets.has_value(); }
  [[nodiscard]] auto targets() -> std::optional<TargetSelector>& { return _targets; }

 private:
  std::vector<Atom>                   _atoms;
  std::optional<Condition>            _condition;
  std::optional<TimedLatchCollection> _duration;
  std::optional<TargetSelector>       _targets;
};

}  // namespace VIPRA::Behaviors