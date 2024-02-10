#pragma once

#include <functional>

#include "vipra/vipra_behaviors/conditions/condition.hpp"
#include "vipra/vipra_behaviors/definitions/behavior_context.hpp"
#include "vipra/vipra_behaviors/events/event_status.hpp"
#include "vipra/vipra_behaviors/util/bool_latch.hpp"

namespace VIPRA::Behaviors {

/**
   * @brief An Event is something that occurs during a simulation, when an event starts it notifies it's subscribing functions and same when it ends
   *
   */
template <typename cond_t>
class Event {
  DEFAULT_CONSTRUCTIBLE(Event)
  COPYABLE(Event)
  MOVEABLE(Event)

 public:
  explicit Event(std::string);

  void evaluate(auto);

  void set_start_condition(cond_t const&);
  void set_end_condition(cond_t const&);

  [[nodiscard]] auto is_occurring() const -> bool;
  [[nodiscard]] auto has_occurred() const -> bool;
  [[nodiscard]] auto is_starting() const -> bool;
  [[nodiscard]] auto is_ending() const -> bool;

  void               set_status(EventStatus);
  [[nodiscard]] auto get_status() const -> EventStatus const&;

  [[nodiscard]] auto get_name() const -> std::string const&;

 private:
  std::string _name;
  EventStatus _status{EventStatus::NOT_OCCURRING};
  bool        _occurred = false;

  Latch                 _latch;
  cond_t                _startCondition;
  std::optional<cond_t> _endCondition;
};
}  // namespace VIPRA::Behaviors
