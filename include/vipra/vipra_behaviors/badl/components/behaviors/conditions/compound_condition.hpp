#pragma once

#include <vector>

#include "badl/actuators/func_call.hpp"
#include "badl/environment/environment.hpp"

#include "interface/prog_interface.hpp"
#include "vipra/vipra_behaviors/senses/sight.hpp"
#include "vipra/vipra_behaviors/senses/sound.hpp"

namespace BADL {
class Agent;

class CompoundCondition {
 public:
  enum class BoolOp { AND, OR };

  [[nodiscard]] auto evaluate(
      BADL::Agent const& agent, BADL::ProgramInterface const& interface,
      BADL::Environment<VIPRA::Sound, VIPRA::Sight> const& environment,
      BADL::time                                           time) const -> bool;

  void add_operation(BoolOp oper);
  void add_query(QueryCall const& call);

 private:
  std::vector<QueryCall> _conditions;
  std::vector<BoolOp>    _steps;

  /**
 * @brief true if the boolean expression can be short circuited
 * 
 * @param met 
 * @param oper 
 * @return true 
 * @return false 
 */
  static inline constexpr auto can_short_circuit(bool met, BoolOp oper) -> bool
  {
    if ( oper == BoolOp::AND && ! met ) {
      return true;
    }
    if ( oper == BoolOp::OR && met ) {
      return true;
    }

    return false;
  }
};
}  // namespace BADL