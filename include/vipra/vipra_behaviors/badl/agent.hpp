
#pragma once

#include "badl/actuators/actuators.hpp"
#include "badl/cognition/behaviors/dsl_behavior.hpp"

#include "badl/beliefs/beliefs.hpp"

#include "badl/cognition/decision_making.hpp"
#include "badl/definitions/time.hpp"

#include "badl/filter/filter.hpp"

#include "badl/cognition/memory.hpp"

#include "vipra/vipra_behaviors/actuators/legs.hpp"
#include "vipra/vipra_behaviors/actuators/mouth.hpp"
#include "vipra/vipra_behaviors/beliefs/identity.hpp"
#include "vipra/vipra_behaviors/beliefs/position.hpp"
#include "vipra/vipra_behaviors/senses/sight.hpp"
#include "vipra/vipra_behaviors/senses/sound.hpp"

namespace BADL {
class Agent {
 public:
  template <typename stimulus_t>
  void apply_stimulus(stimulus_t&& stimulus, BADL::time time)
  {
    if ( ! _filter.gate<stimulus_t>(stimulus) ) return;
    _memory.add_memory(stimulus, time);
  }

  void decide(BADL::Environment<VIPRA::Sound, VIPRA::Sight> const& environment,
              BADL::time                                           time);
  void take_action(BADL::ProgramInterface const&                  interface,
                   BADL::Environment<VIPRA::Sound, VIPRA::Sight>& environment,
                   BADL::time                                     time);

  template <typename belief_t>
  void update_belief(belief_t&& value);
  template <typename belief_t>
  [[nodiscard]] auto get_belief() const noexcept -> belief_t const&;

  template <typename component_t>
  [[nodiscard]] auto get_component() -> component_t&;
  template <typename component_t>
  [[nodiscard]] auto get_component() const -> component_t const&
  {
    return get_component<component_t>();
  }

 private:
  BADL::AttentionFilter<VIPRA::Sight, VIPRA::Sound> _filter;
  BADL::Memory<VIPRA::Sight, VIPRA::Sound>          _memory;
  BADL::Beliefs<VIPRA::Identity, VIPRA::Position>   _beliefs;
  BADL::DecisionMaking<BADL::DSLBehavior>           _decisions;
  BADL::Actuators<VIPRA::Legs, VIPRA::Mouth>        _actuators;
};
}  // namespace BADL

#define BADL_AGENT_MAIN_HPP

#include "agent_detail.hpp"

#undef BADL_AGENT_MAIN_HPP
