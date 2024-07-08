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

  void operator()(auto pack, const VIPRA::idxVec& peds, std::vector<Target> const& targets,
                  std::vector<bool>& met, std::vector<bool> const& /*unused*/, BoolOp /*unused*/) const
  {
    for ( auto idx : peds ) {
      auto const& coords = pack.pedset.all_coords();
      met[idx] = coords[idx].distance_to(coords[targets[idx].targetIdx]) <= _within.value(idx);
    }
  }

 private:
  NumericValue _within;
};
}  // namespace VIPRA::Behaviors
