#pragma once

#include <vector>

#include "badl/environment/environment.hpp"

#include "vipra/vipra_behaviors/senses/sight.hpp"
#include "vipra/vipra_behaviors/senses/sound.hpp"

#include "sub_condition.hpp"

namespace BADL {
class Condition {
 public:
  [[nodiscard]] auto evaluate(
      BADL::Memory const&                                    memory,
      BADL::Beliefs<VIPRA::Identity, VIPRA::Position> const& beliefs,
      BADL::Environment<VIPRA::Sound, VIPRA::Sight> const&   environment,
      BADL::time                                             time) -> bool;

  void add_operation(BoolOp oper);
  void add_subcondition(SubCondition&& condition);

 private:
  std::vector<SubCondition> _conditions;
  std::vector<BoolOp>       _steps;
};
}  // namespace BADL