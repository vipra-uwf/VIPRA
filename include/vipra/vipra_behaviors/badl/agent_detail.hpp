#pragma once

#ifndef BADL_AGENT_MAIN_HPP
#include "agent.hpp"
#endif

namespace BADL {
template <typename belief_t>
void Agent::update_belief(belief_t&& value)
{
  auto& belief = _beliefs.get<belief_t>();
  belief = std::forward<belief_t>(value);
}

template <typename belief_t>
auto Agent::get_belief() const noexcept -> belief_t const&
{
  return _beliefs.get<belief_t>();
}

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

  // TODO(rolland): add in memory of action taken
  // _memory.add_memory(actionTaken, time);
}
}  // namespace BADL
