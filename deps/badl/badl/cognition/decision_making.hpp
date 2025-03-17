#pragma once

#include <tuple>
#include <type_traits>

#include "badl/program_interface.hpp"

#include "badl/actions/action.hpp"
#include "badl/components/base_component.hpp"
#include "badl/components/component.hpp"
#include "badl/environment/environment.hpp"

namespace BADL {
class Agent;

template <typename... thought_ts>
class DecisionMaking
    : public BADL::Component,
      public BADL::BaseComponent<DecisionMaking<thought_ts...>> {
 public:
  void decide(BADL::Agent const& agent, BADL::ProgramInterface const& interface,
              BADL::Environment<BADL_STIMULI_TYPES> const& environment,
              BADL::time                                   time);
  void act(BADL::Agent& agent, BADL::ProgramInterface const& interface,
           BADL::Environment<BADL_STIMULI_TYPES>& environment, BADL::time time);

  template <typename process_t>
  auto get() noexcept -> process_t&
  {
    return std::get<std::remove_cvref_t<process_t>>(_components);
  }
  template <typename process_t>
  auto get() const noexcept -> process_t const&
  {
    return std::get<std::remove_cvref_t<process_t>>(_components);
  }

 private:
  BADL::Action const*       _nextAction{};
  std::tuple<thought_ts...> _components;
};
}  // namespace BADL

#define BADL_DECISION_MAKING_MAIN_HPP

#include "./_detail/decision_making_detail.hpp"

#undef BADL_DECISION_MAKING_MAIN_HPP