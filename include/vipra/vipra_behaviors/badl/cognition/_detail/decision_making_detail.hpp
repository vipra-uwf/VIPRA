#pragma once

#ifndef BADL_DECISION_MAKING_MAIN_HPP
#include "../decision_making.hpp"
#endif

namespace BADL {
template <typename... thought_ts>
void DecisionMaking<thought_ts...>::decide(
    BADL::Memory<VIPRA::Sight, VIPRA::Sound> const&        memory,
    BADL::Beliefs<VIPRA::Identity, VIPRA::Position> const& beliefs,
    BADL::Environment<VIPRA::Sound, VIPRA::Sight> const&   environment,
    BADL::time                                             time)
{
  // go through each process and find the action with the most utility
  std::apply(
      [&](auto& process) {
        auto action = process.decide_action(memory, beliefs, environment, time);
        if ( action.utility() > _nextAction->utility() ) {
          _nextAction = &action;
        }
      },
      _thoughtprocesses);
}

template <typename... thought_ts>
void DecisionMaking<thought_ts...>::act(
    BADL::ProgramInterface const&                  interface,
    BADL::Environment<VIPRA::Sound, VIPRA::Sight>& environment)
{
  return (*_nextAction)(interface, environment);
}
}  // namespace BADL