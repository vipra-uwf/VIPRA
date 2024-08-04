#pragma once

#include <string>

namespace VIPRA::Modules {

enum class Type {
  MODEL,
  GOALS,
  INPUT,
  OUTPUT,
  PEDESTRIANS,
  PARAMETERS,
  OBSTACLES,
  SIMULATION,
  BEHAVIOR_MODEL,
};

inline auto to_string(Type type) -> std::string
{
  switch ( type ) {
    case Type::MODEL:
      return "model";
    case Type::GOALS:
      return "goals";
    case Type::INPUT:
      return "input";
    case Type::OUTPUT:
      return "output";
    case Type::PEDESTRIANS:
      return "pedestrians";
    case Type::PARAMETERS:
      return "parameters";
    case Type::OBSTACLES:
      return "obstacles";
    case Type::SIMULATION:
      return "simulation";
    case Type::BEHAVIOR_MODEL:
      return "behavior_model";
  }

  return "unknown";
}
}  // namespace VIPRA::Modules