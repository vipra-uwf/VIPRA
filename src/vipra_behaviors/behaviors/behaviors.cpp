

#include "badl/components/behaviors/behaviors.hpp"
#include "badl/actions/action.hpp"

namespace BADL {
auto Behaviors::decide_action(
    BADL::Agent const& agent, BADL::ProgramInterface const& interface,
    BADL::Environment<VIPRA::Sound, VIPRA::Sight> const& environment,
    BADL::time time) -> BADL::Action const&
{
  BADL::Action const* action = &BADL::empty_action();

  for ( auto const& behavior : _behaviors ) {
    auto const* currAction =
        &behavior.decide_action(agent, interface, environment, time);
    if ( currAction->utility() > action->utility() ) {
      action = currAction;
    }
  }

  return *action;
}
}  // namespace BADL