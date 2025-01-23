#pragma once

#ifndef BADL_DECISION_MAKING_MAIN_HPP
#include "../decision_making.hpp"
#endif

namespace BADL {
template <typename... thought_ts>
void DecisionMaking<thought_ts...>::decide(
    BADL::Agent const& agent, BADL::ProgramInterface const& interface,
    BADL::Environment<VIPRA::Sound, VIPRA::Sight> const& environment,
    BADL::time                                           time)
{
  // go through each process and find the action with the most utility
  std::apply(
      [&](auto& process) {
        auto action =
            process.decide_action(agent, interface, environment, time);
        if ( action.utility() > _nextAction->utility() ) {
          _nextAction = &action;
        }
      },
      _thoughtprocesses);
}

template <typename... thought_ts>
void DecisionMaking<thought_ts...>::act(
    BADL::Agent& agent, BADL::ProgramInterface const& interface,
    BADL::Environment<VIPRA::Sound, VIPRA::Sight>& environment, BADL::time time)
{
  return (*_nextAction)(agent, interface, environment, time);
}
}  // namespace BADL