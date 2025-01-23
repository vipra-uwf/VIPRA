#pragma once

#include <string>
#include <variant>

#include "badl/environment/environment.hpp"

#include "vipra/vipra_behaviors/interface/prog_interface.hpp"
#include "vipra/vipra_behaviors/senses/sight.hpp"
#include "vipra/vipra_behaviors/senses/sound.hpp"

namespace BADL {
class Agent;

// NOLINTBEGIN(cppcoreguidelines-avoid-const-or-ref-data-members)
struct ComponentParams {
  std::vector<std::variant<int, float, std::string>> parameters;

  [[nodiscard]] auto operator[](size_t idx) noexcept
      -> std::variant<int, float, std::string>&
  {
    assert(idx < parameters.size());
    return parameters[idx];
  }
  [[nodiscard]] auto operator[](size_t idx) const noexcept
      -> std::variant<int, float, std::string> const&
  {
    assert(idx < parameters.size());
    return parameters[idx];
  }

  template <typename value_t>
  [[nodiscard]] auto get(size_t idx) noexcept -> value_t&
  {
    assert(idx < parameters.size());
    return std::get<value_t>(parameters[idx]);
  }

  template <typename value_t>
  [[nodiscard]] auto get(size_t idx) const noexcept -> value_t&
  {
    assert(idx < parameters.size());
    return std::get<value_t>(parameters[idx]);
  }
};
// NOLINTEND(cppcoreguidelines-avoid-const-or-ref-data-members)

using ConditionFunc =
    bool (*)(BADL::Agent const&, BADL::ProgramInterface const&,
             BADL::Environment<VIPRA::Sound, VIPRA::Sight> const&,
             BADL::ComponentParams const&);

using ActuatorFunc = void (*)(BADL::ProgramInterface const&,
                              BADL::Environment<VIPRA::Sound, VIPRA::Sight>&,
                              BADL::ComponentParams const&);

// TODO(rolland): figure this part out
using InternalFunc = void (*)();

}  // namespace BADL