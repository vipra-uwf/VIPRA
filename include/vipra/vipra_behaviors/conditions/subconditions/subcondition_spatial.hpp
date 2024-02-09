#ifndef VIPRA_SUBCONDITION_SPATIAL_HPP
#define VIPRA_SUBCONDITION_SPATIAL_HPP

#include <utility>

#include "conditions/sub_condition.hpp"
#include "util/class_types.hpp"
#include "values/numeric_value.hpp"

namespace BHVR {
class SubConditionSpatial {
  NON_DEFAULT_CONSTRUCTIBLE(SubConditionSpatial)
  COPYABLE(SubConditionSpatial)
  MOVEABLE(SubConditionSpatial)
 public:
  explicit SubConditionSpatial(NumericValue value) : _within(std::move(value)) {}

  void operator()(Simpack, const VIPRA::idxVec&, const std::vector<Target>&, std::vector<bool>&,
                  const std::vector<bool>&, BoolOp) const;

 private:
  NumericValue _within;
};
}  // namespace BHVR

#endif