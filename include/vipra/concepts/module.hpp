#pragma once

#include <concepts>
#include <cstddef>
#include <utility>

#include "vipra/concepts/parameters.hpp"
#include "vipra/macros/module.hpp"
#include "vipra/types/parameter.hpp"

namespace VIPRA::Concepts {

/**
 * @brief Checks that a module type tag is valid
 * 
 * @tparam module_t 
 * @tparam type 
 */
template <typename module_t, Modules::Type type>
concept is_type = module_t::_VIPRA_MODULE_TYPE_ == type;

/**
 * @brief Checks that a type has a register_params method
 * 
 * @tparam module_t 
 */
template <typename module_t>
concept has_parameters = requires(module_t module, DummyParams& params) {
  { module.register_params(params) } -> std::same_as<void>;
};

/**
 * @brief Checks that a type has a config method
 * 
 * @tparam module_t 
 */
template <typename module_t>
concept has_config = requires(module_t module, DummyParams const& params, VIPRA::Random::Engine& engine) {
  { module.config(params, engine) } -> std::same_as<void>;
};

/**
 * @brief Checks that a type is a module
 * 
 * @tparam module_t 
 * @tparam type 
 */
template <typename module_t, Modules::Type type>
concept is_module = is_type<module_t, type> && has_parameters<module_t> && has_config<module_t>;
}  // namespace VIPRA::Concepts