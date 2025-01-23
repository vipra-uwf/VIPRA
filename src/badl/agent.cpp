
#include "badl/agent.hpp"

namespace BADL {
inline void Agent::decide(
    BADL::Environment<VIPRA::Sound, VIPRA::Sight> const& environment,
    BADL::time                                           time)
{
  _decisions.decide(_memory, _beliefs, environment, time);
}

inline void Agent::take_action(
    BADL::ProgramInterface const&                  interface,
    BADL::Environment<VIPRA::Sound, VIPRA::Sight>& environment, BADL::time time)
{
  _decisions.act(interface, environment);
}
}  // namespace BADL
