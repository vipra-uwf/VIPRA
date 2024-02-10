#pragma once

#include <attributes/attributes.hpp>
#include <targets/target.hpp>

namespace VIPRA::Behaviors {
struct AtomScale {
  Attribute       attribute;
  CAttributeValue value;

  inline void operator()(Simpack pack, const VIPRA::idxVec& peds, std::vector<bool> const& conditionMet,
                         std::vector<Target> const& targets) const {
    for (VIPRA::idx idx = 0; idx < peds.size(); ++idx) {
      if (conditionMet[idx]) AttributeHandling::scale_value(targets[idx], attribute, pack, pack.state, value);
    }
  }
};
}  // namespace VIPRA::Behaviors
