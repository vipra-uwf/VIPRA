#pragma once

#include "vipra/geometry/f3d.hpp"

#include "vipra/vipra_behaviors/conditions/sub_condition.hpp"
#include "vipra/vipra_behaviors/definitions/dsl_types.hpp"
#include "vipra/vipra_behaviors/definitions/sim_pack.hpp"
#include "vipra/vipra_behaviors/events/event.hpp"
#include "vipra/vipra_behaviors/locations/location.hpp"
#include "vipra/vipra_behaviors/time/time.hpp"
#include "vipra/vipra_behaviors/util/class_types.hpp"
#include "vipra/vipra_behaviors/util/timed_latch.hpp"
#include "vipra/vipra_behaviors/values/numeric_value.hpp"

namespace VIPRA::Behaviors {
class SubConditionEnter {
  NON_DEFAULT_CONSTRUCTIBLE(SubConditionEnter)
  COPYABLE(SubConditionEnter)
  MOVEABLE(SubConditionEnter)

 public:
  explicit SubConditionEnter(VIPRA::idx location) : _location{location} {}

  void operator()(auto pack, const VIPRA::idxVec& peds, std::vector<Target> const& targets,
                  std::vector<bool>& met, std::vector<bool> const& /*unused*/, BoolOp /*unused*/) {
    if (_entered.size() < pack.get_pedset().getNumPedestrians())
      _entered.resize(pack.get_pedset().getNumPedestrians());

    for (auto ped : peds) {
      if (_entered[targets[ped].targetIdx]) {
        met[ped] = false;
      }

      Location& loc = pack.get_context().locations[_location];
      bool      enter = loc.contains(pack.get_state().coords[targets[ped].targetIdx]) &&
                   !loc.contains(pack.get_pedset().getPedCoords(targets[ped].targetIdx));

      if (enter) {
        _entered[targets[ped].targetIdx] = true;
      }

      met[ped] = enter;
    }
  }

 private:
  VIPRA::idx _location;

  // TODO(rolland): temporary fix for not having pedestrian memory
  std::vector<bool> _entered;
};
}  // namespace VIPRA::Behaviors