#pragma once

#include "badl/components/component_methods.hpp"

namespace BADL {
struct ActivationCall {
  ActivationFunc  method = nullptr;
  ComponentParams parameters;

  void operator()(BADL::Agent& agent, BADL::ProgramInterface const& interface,
                  BADL::Environment<BADL_STIMULI_TYPES>& environment,
                  BADL::time                             time) const
  {
    assert(method != nullptr);
    method(agent, interface, environment, parameters, time);
  }
};

struct QueryCall {
  QueryFunc       method = nullptr;
  ComponentParams parameters;

  [[nodiscard]] auto operator()(
      BADL::Agent const& agent, BADL::ProgramInterface const& interface,
      BADL::Environment<BADL_STIMULI_TYPES> const& environment,
      BADL::time                                   time) const -> bool
  {
    assert(method != nullptr);
    return method(agent, interface, environment, parameters, time);
  }
};
}  // namespace BADL