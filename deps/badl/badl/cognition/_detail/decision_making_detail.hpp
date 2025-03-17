#pragma once

#include "badl/actions/action.hpp"
#ifndef BADL_DECISION_MAKING_MAIN_HPP
#include "../decision_making.hpp"
#endif

namespace BADL {
template <typename... thought_ts>
void DecisionMaking<thought_ts...>::decide(
    BADL::Agent const& agent, BADL::ProgramInterface const& interface,
    BADL::Environment<BADL_STIMULI_TYPES> const& environment, BADL::time time)
{
  _nextAction = &BADL::empty_action();
  // go through each process and find the action with the most utility
  if constexpr ( std::tuple_size_v<decltype(_components)> != 0 )
    std::apply(
        [&](auto& process) {
          auto const* action =
              &process.decide_action(agent, interface, environment, time);
          if ( action->utility() > _nextAction->utility() ) {
            _nextAction = action;
          }
        },
        _components);
}

template <typename... thought_ts>
void DecisionMaking<thought_ts...>::act(
    BADL::Agent& agent, BADL::ProgramInterface const& interface,
    BADL::Environment<BADL_STIMULI_TYPES>& environment, BADL::time time)
{
  (*_nextAction)(agent, interface, environment, time);
}
}  // namespace BADL