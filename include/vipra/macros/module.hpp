#pragma once

#define VIPRA_MODULE_TYPE(type) \
  constexpr static VIPRA::Modules::Type _VIPRA_MODULE_TYPE_ = VIPRA::Modules::Type::type;

#define VIPRA_MODULE_NAME(name) constexpr static const char* _VIPRA_MODULE_NAME_ = name;

#define CHECK_MODULE(type, ...) static_assert(VIPRA::Concepts::type<__VA_ARGS__>);

// TODO(rolland): add macro for naming a new module