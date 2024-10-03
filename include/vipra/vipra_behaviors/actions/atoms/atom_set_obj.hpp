#pragma once

#include "vipra/vipra_behaviors/targets/target.hpp"

namespace VIPRA::Behaviors {
/**
   * @brief Behavior atom that sets an attribute to a provided value
   * 
   */
struct AtomSetObjective {
  std::string objName;

  void operator()(Simpack pack, const VIPRA::idxVec& peds,
                  std::vector<bool> const&   conditionMet,
                  std::vector<Target> const& targets) const;
};
}  // namespace VIPRA::Behaviors