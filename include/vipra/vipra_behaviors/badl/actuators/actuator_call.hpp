#pragma once

#include "badl/components/component_methods.hpp"

#include "vipra/vipra_behaviors/interface/prog_interface.hpp"
#include "vipra/vipra_behaviors/senses/sight.hpp"
#include "vipra/vipra_behaviors/senses/sound.hpp"

namespace BADL {
struct ActuatorCall {
  ActuatorFunc    method = nullptr;
  ComponentParams parameters;

  void operator()(
      BADL::ProgramInterface const&                  interface,
      BADL::Environment<VIPRA::Sound, VIPRA::Sight>& environment) const
  {
    assert(method != nullptr);
    method(interface, environment, parameters);
  }
};
}  // namespace BADL