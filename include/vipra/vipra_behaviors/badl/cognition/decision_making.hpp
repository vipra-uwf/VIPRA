#pragma once

#include <tuple>

#include "badl/actions/action.hpp"
#include "badl/beliefs/beliefs.hpp"
#include "badl/cognition/memory.hpp"
#include "badl/definitions/time.hpp"

#include "vipra/vipra_behaviors/beliefs/identity.hpp"
#include "vipra/vipra_behaviors/beliefs/position.hpp"
#include "vipra/vipra_behaviors/senses/sight.hpp"
#include "vipra/vipra_behaviors/senses/sound.hpp"

namespace BADL {
template <typename... thought_ts>
class DecisionMaking {
 public:
  void decide(BADL::Memory const&                                    memory,
              BADL::Beliefs<VIPRA::Identity, VIPRA::Position> const& beliefs,
              BADL::Environment<VIPRA::Sound, VIPRA::Sight> const& environment,
              BADL::time                                           time);
  void act(BADL::ProgramInterface const&                  interface,
           BADL::Environment<VIPRA::Sound, VIPRA::Sight>& environment);

 private:
  BADL::Action const*       _nextAction;
  std::tuple<thought_ts...> _thoughtprocesses;
};
}  // namespace BADL

#define BADL_DECISION_MAKING_MAIN_HPP

#include "./_detail/decision_making_detail.hpp"

#undef BADL_DECISION_MAKING_MAIN_HPP