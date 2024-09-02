#pragma once

#include <string>

namespace VIPRA::Modules {

// NOLINTBEGIN(readability-identifier-naming)
enum class Type {
  Model,
  Goals,
  Input,
  Output,
  Pedestrians,
  Parameters,
  Obstacles,
  Simulation,
  Behavior_model,
};
// NOLINTEND(readability-identifier-naming)

inline auto to_string(Type type) -> std::string
{
  switch ( type ) {
    case Type::Model:
      return "model";
    case Type::Goals:
      return "goals";
    case Type::Input:
      return "input";
    case Type::Output:
      return "output";
    case Type::Pedestrians:
      return "pedestrians";
    case Type::Parameters:
      return "parameters";
    case Type::Obstacles:
      return "obstacles";
    case Type::Simulation:
      return "simulation";
    case Type::Behavior_model:
      return "behavior_model";
  }

  return "unknown";
}
}  // namespace VIPRA::Modules