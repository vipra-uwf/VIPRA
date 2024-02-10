#pragma once

namespace VIPRA::Behaviors {
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
}  // namespace VIPRA::Behaviors
