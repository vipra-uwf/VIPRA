#pragma once

#include <concepts>
#include <cstddef>
#include <utility>

#include "vipra/concepts/parameters.hpp"
#include "vipra/macros/module.hpp"
#include "vipra/types/parameter.hpp"

namespace VIPRA::Concepts {

template <typename module_t, Modules::Type type>
concept is_type = module_t::_VIPRA_MODULE_TYPE_ == type;

template <typename module_t>
concept has_parameters = requires(module_t module, DummyParams& params) {
  { module.register_params(params) } -> std::same_as<void>;
};

template <typename module_t>
concept has_config = requires(module_t module, DummyParams const& params, VIPRA::Random::Engine& engine) {
  { module.config(params, engine) } -> std::same_as<void>;
};

template <typename module_t, Modules::Type type>
concept is_module = is_type<module_t, type> && has_parameters<module_t> && has_config<module_t>;
}  // namespace VIPRA::Concepts