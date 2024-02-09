#ifndef VIPRA_BEHAVIORS_ATOM_SET_HPP
#define VIPRA_BEHAVIORS_ATOM_SET_HPP

#include <attributes/attributes.hpp>
#include "targets/target.hpp"

namespace BHVR {
struct AtomSet {
  Attribute       attribute;
  CAttributeValue value;

  inline void operator()(Simpack pack, const VIPRA::idxVec& peds, const std::vector<bool>& conditionMet,
                         const std::vector<Target>& targets) const {
    for (VIPRA::idx idx = 0; idx < peds.size(); ++idx) {
      if (conditionMet[idx]) {
        AttributeHandling::set_value(targets[idx], attribute, pack, pack.state, value);
      }
    }
  }
};
}  // namespace BHVR

#endif