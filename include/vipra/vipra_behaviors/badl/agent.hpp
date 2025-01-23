
#pragma once

#include "badl/actuators/actuators.hpp"

#include "badl/components/behaviors/behaviors.hpp"
#include "badl/components/beliefs/beliefs.hpp"
#include "badl/components/cognition/decision_making.hpp"
#include "badl/components/filter/filter.hpp"
#include "badl/components/memory/memory.hpp"

#include "badl/definitions/time.hpp"

#include "vipra/vipra_behaviors/actuators/legs.hpp"
#include "vipra/vipra_behaviors/actuators/mouth.hpp"
#include "vipra/vipra_behaviors/beliefs/identity.hpp"
#include "vipra/vipra_behaviors/beliefs/position.hpp"
#include "vipra/vipra_behaviors/senses/sight.hpp"
#include "vipra/vipra_behaviors/senses/sound.hpp"

namespace BADL {
class Agent {
 public:
  template <typename component_t>
  [[nodiscard]] auto get_component() const -> component_t const&
  {
    return get_component<component_t>();
  }

  void decide(BADL::ProgramInterface const&                        interface,
              BADL::Environment<VIPRA::Sound, VIPRA::Sight> const& environment,
              BADL::time                                           time);
  void take_action(BADL::ProgramInterface const&                  interface,
                   BADL::Environment<VIPRA::Sound, VIPRA::Sight>& environment,
                   BADL::time                                     time);

 private:
  BADL::AttentionFilter<VIPRA::Sight, VIPRA::Sound> _filter;
  BADL::Memory                                      _memory;
  BADL::Beliefs<VIPRA::Identity, VIPRA::Position>   _beliefs;
  BADL::DecisionMaking<BADL::Behaviors>             _decisions;
  BADL::Actuators<VIPRA::Legs, VIPRA::Mouth>        _actuators;

 public:
  template <typename stimulus_t>
  void apply_stimulus(stimulus_t&& stimulus, BADL::time time)
  {
    if ( ! _filter.gate<stimulus_t>(stimulus) ) return;
    _memory.add_memory(stimulus, time);
  }

  template <typename component_t>
  [[nodiscard]] auto get_component() -> component_t&
  {
    if constexpr ( std::is_same_v<
                       component_t,
                       BADL::AttentionFilter<VIPRA::Sight, VIPRA::Sound>> )
      return _filter;
    if constexpr ( std::is_same_v<component_t, BADL::Memory> ) return _memory;
    if constexpr ( std::is_same_v<component_t, BADL::Beliefs<VIPRA::Identity,
                                                             VIPRA::Position>> )
      return _beliefs;
    if constexpr ( std::is_same_v<component_t,
                                  BADL::DecisionMaking<BADL::Behaviors>> )
      return _decisions;
    if constexpr ( std::is_same_v<component_t,
                                  BADL::Actuators<VIPRA::Legs, VIPRA::Mouth>> )
      return _actuators;
  }
};
}  // namespace BADL
