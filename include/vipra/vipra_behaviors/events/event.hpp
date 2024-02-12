#pragma once

#include <functional>
#include <optional>

#include "vipra/types/idx.hpp"
#include "vipra/vipra_behaviors/events/event_status.hpp"
#include "vipra/vipra_behaviors/util/bool_latch.hpp"
#include "vipra/vipra_behaviors/util/timed_latch.hpp"

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
  explicit Event(std::string name) : _name(std::move(name)) {}

  void evaluate(auto pack);

  void set_start_condition(cond_t const& condition) { _startCondition = condition; }
  void set_end_condition(cond_t const& condition) { _endCondition = condition; }

  [[nodiscard]] auto is_occurring() const -> bool {
    return _status == EventStatus::OCCURRING || _status == EventStatus::STARTING;
  }
  [[nodiscard]] auto has_occurred() const -> bool { return _occurred; }
  [[nodiscard]] auto is_starting() const -> bool { return _status == EventStatus::STARTING; }
  [[nodiscard]] auto is_ending() const -> bool { return _status == EventStatus::ENDING; }

  void               set_status(EventStatus status) { _status = status; }
  [[nodiscard]] auto get_status() const -> EventStatus const& { return _status; }

  [[nodiscard]] auto get_name() const -> std::string const& { return _name; }

 private:
  std::string _name;
  EventStatus _status{EventStatus::NOT_OCCURRING};
  bool        _occurred = false;

  Latch                 _latch;
  cond_t                _startCondition;
  std::optional<cond_t> _endCondition;
};

template <typename cond_t>
void Event<cond_t>::evaluate(auto pack) {
  if (_status == EventStatus::ENDING) {
    _status = EventStatus::NOT_OCCURRING;
  }
  if (_status == EventStatus::STARTING) {
    _status = EventStatus::OCCURRING;
  }

  std::vector<VIPRA::idx>             peds{0};
  std::vector<bool>                   met{false};
  std::optional<TimedLatchCollection> temp;

  // TODO (rolland) : These might need target selectors?
  if (_status == EventStatus::OCCURRING) {
    if (_endCondition) {
      _endCondition.value().evaluate(pack, peds, met, {}, temp);
      if (met[0]) {
        // spdlog::info("Event {} is Ending", _name);
        _status = EventStatus::ENDING;
      }
    }

    return;
  }

  _startCondition.evaluate(pack, peds, met, {}, temp);
  if (met[0]) {
    // spdlog::info("Event {} is Starting", _name);
    _occurred = true;
    _status = EventStatus::STARTING;
  }
}
}  // namespace VIPRA::Behaviors
