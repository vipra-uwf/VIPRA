#pragma once

#include <cassert>
#include <string>
#include <variant>

#include "badl/build_macros.hpp"

#include "badl/program_interface.hpp"

#include "badl/environment/environment.hpp"

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
  [[nodiscard]] auto get(size_t idx) const noexcept -> value_t const&
  {
    assert(idx < parameters.size());
    return std::get<value_t>(parameters[idx]);
  }
};
// NOLINTEND(cppcoreguidelines-avoid-const-or-ref-data-members)

using QueryFunc = bool (*)(BADL::Agent const&, BADL::ProgramInterface const&,
                           BADL::Environment<BADL_STIMULI_TYPES> const&,
                           BADL::ComponentParams const&, BADL::time);

using ActivationFunc = void (*)(BADL::Agent&, BADL::ProgramInterface const&,
                                BADL::Environment<BADL_STIMULI_TYPES>&,
                                BADL::ComponentParams const&, BADL::time);

// TODO(rolland): figure this part out
using InternalFunc = void (*)();

}  // namespace BADL