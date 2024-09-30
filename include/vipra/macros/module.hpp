#pragma once

// NOLINTBEGIN(unused-includes)
#include "vipra/modules.hpp"
#include "vipra/modules/module.hpp"
#include "vipra/special_modules/parameters.hpp"
// NOLINTEND(unused-includes)

#define VIPRA_MODULE_TYPE(type)                              \
  constexpr auto module_type() const -> VIPRA::Modules::Type \
  {                                                          \
    return VIPRA::Modules::Type::type;                       \
  }

#define VIPRA_MODULE_NAME(name) \
  constexpr auto module_name() const -> const char* { return name; }

#define NEW_VIPRA_MODULE(Name, Type) \
  namespace Type {                   \
  class Name : public VIPRA::Modules::Module<Name>, public VIPRA::Modules::Type

#define VIPRA_REGISTER_MODULE(Name, Type)                             \
  extern "C" auto create_module() -> VIPRA::Modules::Type*            \
  {                                                                   \
    /* NOLINTNEXTLINE */                                              \
    return new Name();                                                \
  }                                                                   \
  extern "C" void setup_module(void* mod, VIPRA::Parameters& paramIn, \
                               VIPRA::Random::Engine& engine)         \
  {                                                                   \
    static_cast<Name*>(mod)->register_params(paramIn);                \
    static_cast<Name*>(mod)->config(paramIn, engine);                 \
  }
