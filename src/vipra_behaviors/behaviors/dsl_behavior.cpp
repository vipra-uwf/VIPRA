
#include "badl/components/behaviors/dsl_behavior.hpp"

namespace BADL {
auto DSLBehavior::decide_action(
    BADL::Agent const& agent, BADL::ProgramInterface const& interface,
    BADL::Environment<VIPRA::Sound, VIPRA::Sight> const& environment,
    BADL::time time) const -> BADL::Action const&
{
  if ( _condition.evaluate(agent, interface, environment, time) )
    return _action;

  return BADL::empty_action();
}
}  // namespace BADL