#ifndef VIPRA_BEHAVIORS_ATOM_SCALE_HPP
#define VIPRA_BEHAVIORS_ATOM_SCALE_HPP

#include <attributes/attributes.hpp>
#include <targets/target.hpp>

namespace BHVR {
struct AtomScale {
  Attribute       attribute;
  CAttributeValue value;

  inline void operator()(Simpack pack, const VIPRA::idxVec& peds, const std::vector<bool>& conditionMet,
                         const std::vector<Target>& targets) const {
    for (VIPRA::idx idx = 0; idx < peds.size(); ++idx) {
      if (conditionMet[idx]) AttributeHandling::scale_value(targets[idx], attribute, pack, pack.state, value);
    }
  }
};
}  // namespace BHVR

#endif