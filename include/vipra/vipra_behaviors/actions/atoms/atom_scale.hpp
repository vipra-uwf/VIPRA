#pragma once

#include "vipra/vipra_behaviors/attributes/attributes.hpp"
#include "vipra/vipra_behaviors/definitions/sim_pack.hpp"
#include "vipra/vipra_behaviors/targets/target.hpp"

namespace VIPRA::Behaviors {
/**
   * @brief Behavior atom that scales an attribute by a provided value
   * 
   */
struct AtomScale {
  Attribute       attribute;
  CAttributeValue value;

  void operator()(Simpack pack, const VIPRA::idxVec& peds,
                  std::vector<bool> const&   conditionMet,
                  std::vector<Target> const& targets) const;
};
}  // namespace VIPRA::Behaviors
