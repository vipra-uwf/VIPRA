
#pragma once

#include <functional>

#include "badl/build_includes.hpp"
#include "badl/build_macros.hpp"

#include "badl/actuators/actuators.hpp"
#include "badl/cognition/attention.hpp"
#include "badl/cognition/decision_making.hpp"
#include "badl/cognition/internal_state.hpp"
#include "badl/definitions/time.hpp"
#include "badl/sensors/sensors.hpp"
#include "badl/util/in_tuple.hpp"

namespace BADL {
class Agent {
 public:
  template <typename component_t>
  [[nodiscard]] auto get_component() const -> component_t const&
  {
    return get_component<component_t>();
  }

  void decide(BADL::ProgramInterface const&                interface,
              BADL::Environment<BADL_STIMULI_TYPES> const& environment,
              BADL::time                                   time)
  {
    _decisions.decide(*this, interface, environment, time);
  }
  void take_action(BADL::ProgramInterface const&          interface,
                   BADL::Environment<BADL_STIMULI_TYPES>& environment,
                   BADL::time                             time)
  {
    _decisions.act(*this, interface, environment, time);
  }

  [[nodiscard]] auto id() const noexcept -> size_t { return _id; }

  void for_component(std::function<void(BADL::Component&)>&& func)
  {
    auto components =
        std::tie(_attention, _internalState, _decisions, _actuators, _sensors);
    std::apply([&](auto&... component) { (func(component), ...); }, components);
  }
  void for_component(std::function<void(BADL::Component const&)>&& func) const
  {
    auto components =
        std::tie(_attention, _internalState, _decisions, _actuators, _sensors);
    std::apply([&](auto const&... component) { (func(component), ...); },
               components);
  }

 private:
  size_t _id{};

  BADL::Attention<BADL_AGENT_ATTENTION_COMPONENTS>            _attention;
  BADL::InternalState<BADL_AGENT_INTERNAL_STATE_COMPONENTS>   _internalState;
  BADL::DecisionMaking<BADL_AGENT_THOUGHT_PROCESS_COMPONENTS> _decisions;
  BADL::Actuators<BADL_AGENT_ACTUATOR_COMPONENTS>             _actuators;
  BADL::Sensors<BADL_AGENT_SENSOR_COMPONENTS>                 _sensors;

 public:
  template <typename stimulus_t>
  void apply_stimulus(stimulus_t&& stimulus, BADL::time time)
  {
    if ( ! _attention.gate<stimulus_t>(stimulus) ) return;
    // TODO(rolland) : apply
  }

  template <typename component_t>
  [[nodiscard]] auto get_component() -> component_t&
  {
    if constexpr ( BADL::is_in_tuple<
                       component_t,
                       std::tuple<BADL_AGENT_ATTENTION_COMPONENTS>>::value )
      return _attention.get_component<component_t>();
    else if constexpr ( BADL::is_in_tuple<
                            component_t,
                            std::tuple<BADL_AGENT_INTERNAL_STATE_COMPONENTS>>::
                            value )
      return _internalState.get_component<component_t>();
    else if constexpr ( BADL::is_in_tuple<
                            component_t,
                            std::tuple<BADL_AGENT_THOUGHT_PROCESS_COMPONENTS>>::
                            value )
      return _decisions.get_component<component_t>();
    else if constexpr ( BADL::is_in_tuple<
                            component_t,
                            std::tuple<BADL_AGENT_ACTUATOR_COMPONENTS>>::value )
      return _actuators.get_component<component_t>();
    else if constexpr ( BADL::is_in_tuple<
                            component_t,
                            std::tuple<BADL_AGENT_SENSOR_COMPONENTS>>::value )
      return _sensors.get_component<component_t>();
    else
      static_assert(false, "Incorrect Component");
  }
};
}  // namespace BADL
