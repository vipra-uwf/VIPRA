#pragma once

#include <cstddef>
#include <vector>

#include "badl/actuators/actuator_call.hpp"

namespace BADL {
class Action {
 public:
  void operator()(
      BADL::ProgramInterface const&                  interface,
      BADL::Environment<VIPRA::Sound, VIPRA::Sight>& environment) const
  {
    for ( auto const& call : _calls ) {
      call.method(interface, environment, call.parameters);
    }
  }

  [[nodiscard]] auto utility() const -> size_t { return _utility; }

 private:
  std::vector<BADL::ActuatorCall> _calls;
  size_t                          _utility{};
};

inline auto empty_action() -> Action const&
{
  static Action empty{};
  return empty;
}
}  // namespace BADL