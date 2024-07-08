#pragma once

#define VIPRA_MODULE_TYPE(type) \
  constexpr auto module_type() const->VIPRA::Modules::Type { return VIPRA::Modules::Type::type; }

#define VIPRA_MODULE_NAME(name) \
  constexpr auto module_name() const->const char* { return name; }
