#pragma once

#include <utility>

#include "vipra/vipra_behaviors/conditions/sub_condition.hpp"
#include "vipra/vipra_behaviors/util/class_types.hpp"
#include "vipra/vipra_behaviors/values/numeric_value.hpp"

namespace VIPRA::Behaviors {
class SubConditionSpatial {
  NON_DEFAULT_CONSTRUCTIBLE(SubConditionSpatial)
  COPYABLE(SubConditionSpatial)
  MOVEABLE(SubConditionSpatial)
 public:
  explicit SubConditionSpatial(NumericValue value) : _within(std::move(value)) {}

  void operator()(auto pack, const VIPRA::idxVec&, std::vector<Target> const&, std::vector<bool>&,
                  std::vector<bool> const&, BoolOp) const;

 private:
  NumericValue _within;
};
}  // namespace VIPRA::Behaviors
