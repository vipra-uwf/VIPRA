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

  void add_activation_call(ActivationCall&& call);
  void add_query_call(QueryCall&& call);

  [[nodiscard]] auto action() noexcept -> BADL::Action& { return _action; }
  [[nodiscard]] auto action() const noexcept -> BADL::Action const&
  {
    return _action;
  }

 private:
  std::string             _name;
  BADL::CompoundCondition _condition;
  BADL::Action            _action;
};
}  // namespace BADL
