#pragma once

#include "vipra/modules.hpp"

#define VIPRA_MODULE_TYPE(type) \
  constexpr auto module_type() const -> VIPRA::Modules::Type { return VIPRA::Modules::Type::type; }

#define VIPRA_MODULE_NAME(name) \
  constexpr auto module_name() const -> const char* { return name; }

#define NEW_VIPRA_MODULE(Name, Type) \
  namespace Type {                   \
  class Name : public VIPRA::Modules::Module<Name>, public VIPRA::Modules::Type<Name>
