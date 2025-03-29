#pragma once

#include <string>
#include <utility>
#include "vipra/types/time.hpp"
#include "vipra/vipra_behaviors/conditions/sub_condition.hpp"
#include "vipra/vipra_behaviors/definitions/sim_pack.hpp"
#include "vipra/vipra_behaviors/util/class_types.hpp"

namespace VIPRA::Behaviors {
class SubConditionEnterObj {
  NON_DEFAULT_CONSTRUCTIBLE(SubConditionEnterObj)
  COPYABLE(SubConditionEnterObj)
  MOVEABLE(SubConditionEnterObj)

 public:
  explicit SubConditionEnterObj(std::string objName) : _objective(std::move(objName)) {}

  void operator()(Simpack pack, const VIPRA::idxVec& peds,
                  std::vector<Target> const& targets, std::vector<bool>& met,
                  std::vector<bool> const& /*unused*/, BoolOp /*unused*/);

 private:
  std::string _objective;

  // TODO(rolland): temporary fix for not having pedestrian memory
  std::vector<std::set<VIPRA::idx>> _visited;
};
}  // namespace VIPRA::Behaviors