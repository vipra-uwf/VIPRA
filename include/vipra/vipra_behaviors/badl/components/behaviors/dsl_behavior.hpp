#pragma once

#include <string>

#include "badl/actions/action.hpp"

#include "badl/actuators/func_call.hpp"
#include "badl/components/behaviors/conditions/compound_condition.hpp"
#include "badl/environment/environment.hpp"

#include "vipra/vipra_behaviors/senses/sight.hpp"
#include "vipra/vipra_behaviors/senses/sound.hpp"

namespace BADL {
/**
 * Describes a specific behavior. Implementations can either define the behavior directly in C++ or use a DSL.
 */
class DSLBehavior {
 public:
  [[nodiscard]] auto decide_action(
      BADL::Agent const&, BADL::ProgramInterface const&,
      BADL::Environment<VIPRA::Sound, VIPRA::Sight> const&,
      BADL::time) const -> BADL::Action const&;

  void add_actuator_call(ActivationCall&& call);

 private:
  std::string             _name;
  BADL::CompoundCondition _condition;
  BADL::Action            _action;
};
}  // namespace BADL
