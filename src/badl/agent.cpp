
#include "badl/agent.hpp"
#include "interface/prog_interface.hpp"

namespace BADL {
void Agent::decide(
    BADL::ProgramInterface const&                        interface,
    BADL::Environment<VIPRA::Sound, VIPRA::Sight> const& environment,
    BADL::time                                           time)
{
  _decisions.decide(*this, interface, environment, time);
}

void Agent::take_action(
    BADL::ProgramInterface const&                  interface,
    BADL::Environment<VIPRA::Sound, VIPRA::Sight>& environment, BADL::time time)
{
  _decisions.act(*this, interface, environment, time);
}
}  // namespace BADL
