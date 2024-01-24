#pragma once

#include <concepts>
#include <cstddef>
#include <utility>

#include "vipra/concepts/parameters.hpp"
#include "vipra/types/parameter.hpp"

namespace VIPRA::Concepts {

template <typename module_t, Modules::Type type>
concept is_type = module_t::MODULE_TYPE == type;

template <typename module_t>
concept has_static_parameters = requires() {
  { module_t::template register_params<DummyParams>() } -> std::same_as<void>;
};

template <typename module_t>
concept has_setup = requires(module_t module, const DummyParams& params) {
  { module.setup(params) } -> std::same_as<void>;
};

template <typename module_t, Modules::Type type>
concept is_module = is_type<module_t, type> && has_static_parameters<module_t> && has_setup<module_t>;
}  // namespace VIPRA::Concepts