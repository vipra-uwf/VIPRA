#pragma once

#include <concepts>
#include <cstddef>

#include "vipra/concepts/parameters.hpp"
#include "vipra/types/parameter.hpp"

namespace VIPRA::Concepts {

template <typename module_t, Modules::Type type>
concept is_type = module_t::MODULE_TYPE == type;

template <typename module_t, Modules::Type type>
concept Module = requires(module_t module, DummyParams& params) {
  is_type<module_t, type>;
  { module.set_params(params) } -> std::same_as<void>;
};
}  // namespace VIPRA::Concepts