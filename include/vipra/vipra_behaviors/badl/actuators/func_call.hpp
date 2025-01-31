#pragma once

#include "badl/components/component_methods.hpp"

#include "vipra/vipra_behaviors/interface/prog_interface.hpp"
#include "vipra/vipra_behaviors/senses/sight.hpp"
#include "vipra/vipra_behaviors/senses/sound.hpp"

namespace BADL {
struct ActivationCall {
  ActivationFunc  method = nullptr;
  ComponentParams parameters;

  void operator()(BADL::Agent& agent, BADL::ProgramInterface const& interface,
                  BADL::Environment<VIPRA::Sound, VIPRA::Sight>& environment,
                  BADL::time                                     time) const
  {
    assert(method != nullptr);
    method(agent, interface, environment, parameters, time);
  }
};

struct QueryCall {
  QueryFunc       method = nullptr;
  ComponentParams parameters;

  void operator()(BADL::Agent const&                             agent,
                  BADL::ProgramInterface const&                  interface,
                  BADL::Environment<VIPRA::Sound, VIPRA::Sight>& environment,
                  BADL::time                                     time) const
  {
    assert(method != nullptr);
    method(agent, interface, environment, parameters, time);
  }
};
}  // namespace BADL