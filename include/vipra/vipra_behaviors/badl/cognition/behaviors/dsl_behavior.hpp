#pragma once

#include <string>

#include "badl/actions/action.hpp"
#include "badl/beliefs/beliefs.hpp"

#include "badl/cognition/behaviors/conditions/condition.hpp"
#include "badl/cognition/memory.hpp"

#include "badl/environment/environment.hpp"

#include "vipra/vipra_behaviors/beliefs/identity.hpp"
#include "vipra/vipra_behaviors/beliefs/position.hpp"
#include "vipra/vipra_behaviors/senses/sight.hpp"
#include "vipra/vipra_behaviors/senses/sound.hpp"

namespace BADL {
/**
 * Describes a specific behavior. Implementations can either define the behavior directly in C++ or use a DSL.
 */
class DSLBehavior {
 public:
  [[nodiscard]] auto decide_action(
      BADL::Memory<VIPRA::Sight, VIPRA::Sound> const&        memory,
      BADL::Beliefs<VIPRA::Identity, VIPRA::Position> const& beliefs,
      BADL::Environment<VIPRA::Sound, VIPRA::Sight> const&   environment,
      BADL::time time) -> BADL::Action const&;

 private:
  std::string     _name;
  BADL::Condition _condition;
  BADL::Action    _action;
};
}  // namespace BADL
