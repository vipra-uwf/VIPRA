#pragma once

#define VIPRA_MODULE_TYPE(type) \
  static constexpr auto module_type()->VIPRA::Modules::Type { return VIPRA::Modules::Type::type; }

#define VIPRA_MODULE_NAME(name) \
  static constexpr auto module_name()->const char* { return name; }
