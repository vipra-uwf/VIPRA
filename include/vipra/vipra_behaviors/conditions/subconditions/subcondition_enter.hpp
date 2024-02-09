#ifndef VIPRA_SUBCONDITION_ENTER
#define VIPRA_SUBCONDITION_ENTER

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
class SubConditionEnter {
  NON_DEFAULT_CONSTRUCTIBLE(SubConditionEnter)
  COPYABLE(SubConditionEnter)
  MOVEABLE(SubConditionEnter)

 public:
  explicit SubConditionEnter(VIPRA::idx location) : _location{location} {}

  void operator()(Simpack, const VIPRA::idxVec&, std::vector<Target> const&, std::vector<bool>&,
                  std::vector<bool> const&, BoolOp);

 private:
  VIPRA::idx _location;

  // TODO: (rolland) temporary fix for not having pedestrian memory
  std::vector<bool> _entered;
};
}  // namespace BHVR

#endif