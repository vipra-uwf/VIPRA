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
};

inline auto to_string(Type type) -> std::string {
  switch (type) {
    case Type::MODEL:
      return "MODEL";
    case Type::GOALS:
      return "GOALS";
    case Type::INPUT:
      return "INPUT";
    case Type::OUTPUT:
      return "OUTPUT";
    case Type::PEDESTRIANS:
      return "PEDESTRIANS";
    case Type::PARAMETERS:
      return "PARAMETERS";
    case Type::OBSTACLES:
      return "OBSTACLES";
    case Type::SIMULATION:
      return "SIMULATION";
  }
}

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage) This is a very simple macro
#define VIPRA_MODULE_TYPE(type)                                                   \
 public:                                                                          \
  constexpr static VIPRA::Modules::Type MODULE_TYPE = VIPRA::Modules::Type::type; \
                                                                                  \
 private:

#define CHECK_MODULE(type, name) static_assert(VIPRA::Concepts::type<name>);
}  // namespace VIPRA::Modules