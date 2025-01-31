
#pragma once

#include "badl/actuators/actuators.hpp"

#include "badl/components/behaviors/behaviors.hpp"
#include "badl/components/beliefs/beliefs.hpp"
#include "badl/components/cognition/decision_making.hpp"
#include "badl/components/component.hpp"
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

  [[nodiscard]] auto id() const noexcept -> size_t { return _id; }

  void for_component(std::function<void(BADL::Component&)>&& func)
  {
    //TODO(rolland): agent build-time replace
    func(_memory);
    func(_beliefs);
    func(_actuators);
  }
  void for_component(std::function<void(BADL::Component const&)>&& func) const
  {
    //TODO(rolland): agent build-time replace
    func(_memory);
    func(_beliefs);
    func(_actuators);
  }

 private:
  size_t                                            _id{};
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
  }

  template <typename component_t>
  [[nodiscard]] auto get_component() -> component_t&
  {
    if constexpr ( std::is_same_v<
                       component_t,
                       BADL::AttentionFilter<VIPRA::Sight, VIPRA::Sound>> )
      return _filter;
    else if constexpr ( std::is_same_v<component_t, BADL::Memory> )
      return _memory;
    else if constexpr ( std::is_same_v<
                            component_t,
                            BADL::Beliefs<VIPRA::Identity, VIPRA::Position>> )
      return _beliefs;
    else if constexpr ( std::is_same_v<component_t,
                                       BADL::DecisionMaking<BADL::Behaviors>> )
      return _decisions;
    else if constexpr ( std::is_same_v<component_t, BADL::Behaviors> )
      return _decisions.get<BADL::Behaviors>();
    else if constexpr ( std::is_same_v<
                            component_t,
                            BADL::Actuators<VIPRA::Legs, VIPRA::Mouth>> )
      return _actuators;
    else
      static_assert(false, "Incorrect Component Type");
  }
};
}  // namespace BADL
