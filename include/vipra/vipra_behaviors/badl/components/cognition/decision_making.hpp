#pragma once

#include <tuple>

#include "badl/actions/action.hpp"
#include "badl/environment/environment.hpp"

#include "vipra/vipra_behaviors/interface/prog_interface.hpp"
#include "vipra/vipra_behaviors/senses/sight.hpp"
#include "vipra/vipra_behaviors/senses/sound.hpp"

namespace BADL {
class Agent;

template <typename... thought_ts>
class DecisionMaking {
 public:
  void decide(BADL::Agent const& agent, BADL::ProgramInterface const& interface,
              BADL::Environment<VIPRA::Sound, VIPRA::Sight> const& environment,
              BADL::time                                           time);
  void act(BADL::Agent& agent, BADL::ProgramInterface const& interface,
           BADL::Environment<VIPRA::Sound, VIPRA::Sight>& environment,
           BADL::time                                     time);

 private:
  BADL::Action const*       _nextAction;
  std::tuple<thought_ts...> _thoughtprocesses;
};
}  // namespace BADL

#define BADL_DECISION_MAKING_MAIN_HPP

#include "./_detail/decision_making_detail.hpp"

#undef BADL_DECISION_MAKING_MAIN_HPP