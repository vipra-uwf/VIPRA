#ifndef SIMULATION_CONTEXT_HPP
#define SIMULATION_CONTEXT_HPP

#include <memory>
#include <vector>

#include "vipra/geometry/f3d.hpp"
#include "vipra/random/random.hpp"
#include "vipra/types/time.hpp"

#include "vipra/vipra_behaviors/definitions/dsl_types.hpp"
#include "vipra/vipra_behaviors/definitions/pedestrian_types.hpp"

#include "vipra/vipra_behaviors/locations/location.hpp"

namespace VIPRA::Behaviors {

template <typename event_t>
struct BehaviorContext {
  VIPRA::delta_t                   elapsedTime = 0;
  VIPRA::Random::Engine            engine;
  std::vector<Behaviors::stateUID> pedStates;
  std::vector<Behaviors::typeUID>  types;
  std::vector<event_t>             events;
  std::vector<Behaviors::Location> locations;
};
}  // namespace VIPRA::Behaviors

#endif