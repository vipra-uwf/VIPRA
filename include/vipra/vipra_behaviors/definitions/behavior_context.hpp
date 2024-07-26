#ifndef SIMULATION_CONTEXT_HPP
#define SIMULATION_CONTEXT_HPP

#include <memory>
#include <vector>

#include "vipra/geometry/f3d.hpp"
#include "vipra/random/random.hpp"
#include "vipra/types/time.hpp"

#include "vipra/vipra_behaviors/conditions/condition.hpp"
#include "vipra/vipra_behaviors/conditions/sub_condition.hpp"
#include "vipra/vipra_behaviors/definitions/dsl_types.hpp"
#include "vipra/vipra_behaviors/definitions/pedestrian_types.hpp"

#include "vipra/vipra_behaviors/definitions/sim_pack.hpp"
#include "vipra/vipra_behaviors/locations/location.hpp"

namespace VIPRA::Behaviors {

template <typename cond_t>
class Event;

/**
 * @brief The context in which a simulation is run. This includes the elapsed time, the random number generator, the current state of pedestrians, and the current events.
 *
 */
template <typename pedset_t, typename obstacles_t, typename goals_t>
struct BehaviorContext {
  using pack_t = Simpack<pedset_t, obstacles_t, goals_t>;
  VIPRA::delta_t                                      elapsedTime = 0;
  VIPRA::Random::Engine                               engine;
  std::vector<Behaviors::stateUID>                    pedStates;
  std::vector<Behaviors::typeUID>                     types;
  std::vector<Event<Condition<SubCondition<pack_t>>>> events;
  std::vector<Behaviors::Location>                    locations;
};
}  // namespace VIPRA::Behaviors

#endif