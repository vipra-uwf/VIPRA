#pragma once

#include <vector>

#include "badl/environment/environment.hpp"

#include "vipra/vipra_behaviors/interface/prog_interface.hpp"
#include "vipra/vipra_behaviors/senses/sight.hpp"
#include "vipra/vipra_behaviors/senses/sound.hpp"

namespace BADL {
// NOLINTBEGIN(cppcoreguidelines-avoid-const-or-ref-data-members)
struct ActuatorParams {
  std::vector<void*> parameters;
};
// NOLINTEND(cppcoreguidelines-avoid-const-or-ref-data-members)

struct ActuatorCall {
  void (*method)(BADL::ProgramInterface const&,
                 BADL::Environment<VIPRA::Sound, VIPRA::Sight>&,
                 BADL::ActuatorParams const&) = nullptr;
  ActuatorParams parameters;

  void operator()(
      BADL::ProgramInterface const&                  interface,
      BADL::Environment<VIPRA::Sound, VIPRA::Sight>& environment) const
  {
    assert(method != nullptr);
    method(interface, environment, parameters);
  }
};
}  // namespace BADL