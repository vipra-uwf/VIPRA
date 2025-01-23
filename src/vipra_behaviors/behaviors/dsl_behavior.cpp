

#include "badl/cognition/behaviors/dsl_behavior.hpp"

#include "badl/cognition/behaviors/behavior_builder/behavior_builder.hpp"

namespace BADL {
auto DSLBehavior::decide_action(
    BADL::Memory const&                                    memory,
    BADL::Beliefs<VIPRA::Identity, VIPRA::Position> const& beliefs,
    BADL::Environment<VIPRA::Sound, VIPRA::Sight> const&   environment,
    BADL::time time) -> BADL::Action const&
{
  if ( _condition.evaluate(memory, beliefs, environment, time) ) return _action;

  return BADL::empty_action();
}
}  // namespace BADL