#pragma once

#include <string>

namespace VIPRA::Modules {

// NOLINTBEGIN(readability-identifier-naming)
enum class Type {
  Model,
  Goals,
  MapInput,
  PedInput,
  Output,
  Pedestrians,
  Parameters,
  Map,
  Simulation,
  Behavior_model,
  CollisionAvoidance,
};
// NOLINTEND(readability-identifier-naming)

inline auto to_string(Type type) -> std::string
{
  switch ( type ) {
    case Type::Model:
      return "model";
    case Type::Goals:
      return "goals";
    case Type::Output:
      return "output";
    case Type::Pedestrians:
      return "pedestrians";
    case Type::Parameters:
      return "parameters";
    case Type::Map:
      return "map";
    case Type::Simulation:
      return "simulation";
    case Type::Behavior_model:
      return "behavior_model";
    case Type::MapInput:
      return "map_input";
    case Type::PedInput:
      return "ped_input";
    case Type::CollisionAvoidance:
      return "collision_avoidance";
  }

  return "unknown";
}

static constexpr size_t MODULE_COUNT = 8;
}  // namespace VIPRA::Modules