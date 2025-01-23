#pragma once

#include <functional>

#include "badl/beliefs/beliefs.hpp"
#include "badl/cognition/memory.hpp"

#include "vipra/vipra_behaviors/beliefs/identity.hpp"
#include "vipra/vipra_behaviors/beliefs/position.hpp"
#include "vipra/vipra_behaviors/senses/sight.hpp"
#include "vipra/vipra_behaviors/senses/sound.hpp"

namespace BADL {
enum class BoolOp { AND, OR };

/**
 * @brief true if the boolean expression can be short circuited
 * 
 * @param met 
 * @param oper 
 * @return true 
 * @return false 
 */
inline constexpr auto can_short_circuit(bool met, BoolOp oper) -> bool
{
  if ( oper == BoolOp::AND && ! met ) {
    return true;
  }
  if ( oper == BoolOp::OR && met ) {
    return true;
  }

  return false;
}

using SubCondition = std::function<bool(
    BADL::Memory const&                                    memory,
    BADL::Beliefs<VIPRA::Identity, VIPRA::Position> const& beliefs)>;
}  // namespace BADL