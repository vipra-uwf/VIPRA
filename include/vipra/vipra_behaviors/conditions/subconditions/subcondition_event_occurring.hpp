#ifndef VIPRA_SUBCONDITION_EVENT_OCCURRING_HPP
#define VIPRA_SUBCONDITION_EVENT_OCCURRING_HPP

#include "conditions/sub_condition.hpp"
#include "definitions/sim_pack.hpp"
#include "events/event.hpp"
#include "util/class_types.hpp"

namespace BHVR {
class SubConditionEventOccurring {
  NON_DEFAULT_CONSTRUCTIBLE(SubConditionEventOccurring)
  COPYABLE(SubConditionEventOccurring)
  MOVEABLE(SubConditionEventOccurring)
 public:
  explicit SubConditionEventOccurring(VIPRA::idx event) : _event(event) {}

  void operator()(Simpack, const VIPRA::idxVec&, const std::vector<Target>&, std::vector<bool>&,
                  const std::vector<bool>&, BoolOp) const;

 private:
  VIPRA::idx _event;
};
}  // namespace BHVR

#endif