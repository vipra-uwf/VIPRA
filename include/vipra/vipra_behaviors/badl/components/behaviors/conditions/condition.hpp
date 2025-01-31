
#pragma once

#include "badl/components/component_methods.hpp"

namespace BADL {
struct Condition {
  QueryFunc       method = nullptr;
  ComponentParams parameters;

  auto operator()(
      BADL::Agent const& agent, BADL::ProgramInterface const& interface,
      BADL::Environment<VIPRA::Sound, VIPRA::Sight> const& environment,
      BADL::time                                           time) const -> bool
  {
    assert(method != nullptr);
    return method(agent, interface, environment, parameters, time);
  }
};
}  // namespace BADL