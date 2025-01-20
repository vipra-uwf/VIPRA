
#pragma once

#include "badl/actuators/actuator_call.hpp"
#include "vipra/vipra_behaviors/interface/prog_interface.hpp"

namespace VIPRA {
struct Legs {
  void move(BADL::ProgramInterface& interface, BADL::ActuatorParams& params) {}
  void stop(BADL::ProgramInterface& interface, BADL::ActuatorParams& params) {}
};
}  // namespace VIPRA