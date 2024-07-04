#pragma once

#include "vipra/vipra_behaviors/attributes/attributes.hpp"
#include "vipra/vipra_behaviors/targets/target.hpp"

namespace VIPRA::Behaviors {
/**
   * @brief Behavior atom that sets an attribute to a provided value
   * 
   */
struct AtomSet {
  Attribute       attribute;
  CAttributeValue value;

  inline void operator()(auto pack, const VIPRA::idxVec& peds, std::vector<bool> const& conditionMet,
                         std::vector<Target> const& targets) const {
    for ( VIPRA::idx idx = 0; idx < peds.size(); ++idx ) {
      if ( conditionMet[idx] ) {
        AttributeHandling::set_value(targets[idx], attribute, pack, pack.state, value);
      }
    }
  }
};
}  // namespace VIPRA::Behaviors