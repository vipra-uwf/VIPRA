#pragma once

#include <cstddef>
#include <limits>
#include <vector>

#include "badl/actuators/func_call.hpp"

namespace BADL {
class Action {
 public:
  void operator()(BADL::Agent& agent, BADL::ProgramInterface const& interface,
                  BADL::Environment<VIPRA::Sound, VIPRA::Sight>& environment,
                  BADL::time                                     time) const
  {
    for ( auto const& call : _calls ) {
      call.method(agent, interface, environment, call.parameters, time);
    }
  }

  [[nodiscard]] auto utility() const -> size_t { return _utility; }

  void add_call(ActivationCall&& call);

 private:
  std::vector<BADL::ActivationCall> _calls;
  size_t _utility{std::numeric_limits<size_t>::min()};
};

inline auto empty_action() -> Action const&
{
  static Action empty{};
  return empty;
}
}  // namespace BADL