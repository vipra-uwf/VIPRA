#ifndef SIMULATION_CONTEXT_HPP
#define SIMULATION_CONTEXT_HPP

#include <vector>

#include "vipra/random/random.hpp"
#include "vipra/types/time.hpp"

#include "vipra/vipra_behaviors/definitions/dsl_types.hpp"
#include "vipra/vipra_behaviors/definitions/pedestrian_types.hpp"

#include "vipra/vipra_behaviors/definitions/sim_pack.hpp"
#include "vipra/vipra_behaviors/locations/location.hpp"

namespace VIPRA::Behaviors {

class Event;

/**
 * @brief The context in which a simulation is run. This includes the elapsed time, the random number generator, the current state of pedestrians, and the current events.
 *
 */

struct BehaviorContext {
  using pack_t = Simpack;
  VIPRA::delta_t                                          elapsedTime = 0;
  VIPRA::Random::Engine                                   engine;
  std::vector<Behaviors::stateUID>                        pedStates;
  std::vector<Behaviors::typeUID>                         types;
  std::vector<Event>                                      events;
  std::vector<Behaviors::Location>                        locations;
  std::map<std::string, std::vector<Behaviors::Location>> objectives;
};
}  // namespace VIPRA::Behaviors

#endif