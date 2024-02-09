#ifndef VIPRA_SUB_CONDITION_HPP
#define VIPRA_SUB_CONDITION_HPP

#include <functional>

#include "definitions/behavior_context.hpp"
#include "definitions/sim_pack.hpp"
#include "goals/goals.hpp"
#include "targets/target.hpp"

namespace BHVR {
enum class BoolOp { AND, OR };

inline constexpr auto short_circuit(VIPRA::idx idx, std::vector<bool>& met, const std::vector<bool>& prevMet,
                                    BoolOp oper) -> bool {
  if (oper == BoolOp::AND && !prevMet[idx]) {
    met[idx] = false;
    return true;
  }
  if (oper == BoolOp::OR && prevMet[idx]) {
    met[idx] = true;
    return true;
  }

  return false;
}

using SubCondition = std::function<void(Simpack, const VIPRA::idxVec&, const std::vector<Target>&,
                                        std::vector<bool>&, const std::vector<bool>&, BoolOp)>;
}  // namespace BHVR

#endif