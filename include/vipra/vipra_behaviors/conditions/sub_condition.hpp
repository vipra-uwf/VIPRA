#pragma once

#include <functional>

#include "vipra/vipra_behaviors/definitions/sim_pack.hpp"
#include "vipra/vipra_behaviors/targets/target.hpp"

namespace VIPRA::Behaviors {
enum class BoolOp { AND, OR };

/**
 * @brief true if the boolean expression can be short circuited
 * 
 * @param idx 
 * @param met 
 * @param prevMet 
 * @param oper 
 * @return true 
 * @return false 
 */
inline constexpr auto short_circuit(VIPRA::idx idx, std::vector<bool>& met,
                                    std::vector<bool> const& prevMet,
                                    BoolOp                   oper) -> bool
{
  if ( oper == BoolOp::AND && ! prevMet[idx] ) {
    met[idx] = false;
    return true;
  }
  if ( oper == BoolOp::OR && prevMet[idx] ) {
    met[idx] = true;
    return true;
  }

  return false;
}

using SubCondition = std::function<void(
    Simpack, VIPRA::idxVec const&, std::vector<Target> const&,
    std::vector<bool>&, std::vector<bool> const&, BoolOp)>;
}  // namespace VIPRA::Behaviors