#ifndef VIPRA_BEHAVIORS_EVENT_STATUS_HPP
#define VIPRA_BEHAVIORS_EVENT_STATUS_HPP

namespace BHVR {
/**
 * @brief Holds the current status of an event
 * 
 */
enum class EventStatus {
  STARTING,
  OCCURRING,
  ENDING,
  NOT_OCCURRING,
};
}  // namespace BHVR

#endif