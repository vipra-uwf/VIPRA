#ifndef VIPRA_BHVR_EVENT_HPP
#define VIPRA_BHVR_EVENT_HPP

#include <functional>

#include <conditions/condition.hpp>
#include <definitions/behavior_context.hpp>
#include <events/event_status.hpp>
#include "definitions/sim_pack.hpp"
#include "util/bool_latch.hpp"

namespace BHVR {

/**
   * @brief An Event is something that occurs during a simulation, when an event starts it notifies it's subscribing functions and same when it ends
   *
   */
class Event {
  DEFAULT_CONSTRUCTIBLE(Event)
  COPYABLE(Event)
  MOVEABLE(Event)

 public:
  explicit Event(std::string);

  void evaluate(Simpack);

  void set_start_condition(const Condition&);
  void set_end_condition(const Condition&);

  [[nodiscard]] auto is_occurring() const -> bool;
  [[nodiscard]] auto has_occurred() const -> bool;
  [[nodiscard]] auto is_starting() const -> bool;
  [[nodiscard]] auto is_ending() const -> bool;

  void               set_status(EventStatus);
  [[nodiscard]] auto get_status() const -> const EventStatus&;

  [[nodiscard]] auto get_name() const -> const std::string&;

 private:
  std::string _name;
  EventStatus _status{EventStatus::NOT_OCCURRING};
  bool        _occurred = false;

  Latch                    _latch;
  Condition                _startCondition;
  std::optional<Condition> _endCondition;
};
}  // namespace BHVR

#endif