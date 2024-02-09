#ifndef VIPRA_SUBCONDITION_ELAPSED_TIME
#define VIPRA_SUBCONDITION_ELAPSED_TIME

#include "conditions/sub_condition.hpp"
#include "definitions/dsl_types.hpp"
#include "definitions/sim_pack.hpp"
#include "events/event.hpp"
#include "time/time.hpp"
#include "util/class_types.hpp"
#include "util/timed_latch.hpp"
#include "values/numeric_value.hpp"
#include "vipra/geometry/f3d.hpp"

namespace BHVR {
class SubConditionElapsedTimeFromEvent {
  NON_DEFAULT_CONSTRUCTIBLE(SubConditionElapsedTimeFromEvent)
  COPYABLE(SubConditionElapsedTimeFromEvent)
  MOVEABLE(SubConditionElapsedTimeFromEvent)
 public:
  SubConditionElapsedTimeFromEvent(BHVR::NumericValue time, VIPRA::idx event)
      : _event(event), _requiredTime(std::move(time)) {}

  void operator()(Simpack, const VIPRA::idxVec&, std::vector<Target> const&, std::vector<bool>&,
                  std::vector<bool> const&, BoolOp);

 private:
  VIPRA::idx         _event{};
  BHVR::NumericValue _requiredTime{};
  VIPRA::time_s      _startTime{};
};
}  // namespace BHVR

#endif