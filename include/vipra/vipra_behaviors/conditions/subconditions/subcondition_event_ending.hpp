#ifndef VIPRA_SUBCONDITION_EVENT_ENDING_HPP
#define VIPRA_SUBCONDITION_EVENT_ENDING_HPP

#include "conditions/sub_condition.hpp"
#include "definitions/sim_pack.hpp"
#include "events/event.hpp"
#include "util/class_types.hpp"

namespace BHVR {
class SubConditionEventEnding {
  NON_DEFAULT_CONSTRUCTIBLE(SubConditionEventEnding)
  COPYABLE(SubConditionEventEnding)
  MOVEABLE(SubConditionEventEnding)
 public:
  explicit SubConditionEventEnding(VIPRA::idx event) : _event(event) {}

  void operator()(Simpack, const VIPRA::idxVec&, const std::vector<Target>&, std::vector<bool>&,
                  const std::vector<bool>&, BoolOp) const;

 private:
  VIPRA::idx _event;
};
}  // namespace BHVR

#endif