#pragma once

#include <concepts>
#include <cstddef>
#include <iostream>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#include "vipra/concepts/has_parameters.hpp"
#include "vipra/debug/debug_do.hpp"
#include "vipra/modules.hpp"
#include "vipra/random/random.hpp"
#include "vipra/util/crtp.hpp"
#include "vipra/util/template_specialization.hpp"

namespace VIPRA::Modules {

/**
 * @brief VIPRA Module Base CRTP Class
 * 
 * @tparam class_t 
 */
template <typename class_t>
class Module : public Util::CRTP<Module<class_t>> {
  using Util::CRTP<Module<class_t>>::self;

 public:
  template <typename paramreader_t>
  void register_params(paramreader_t&& paramIn);

  template <typename paramreader_t>
  void config(paramreader_t& paramIn, VIPRA::Random::Engine& engine);
};

// ----------------------------------------------------------------------------------------------------------------
// --------------------------------------- IMPLEMENTATIONS --------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------

/**
   * @brief Registers the modules parameters with the parameter reader
   * 
   * @tparam paramreader_t 
   * @param paramIn
   */
template <typename class_t>
template <typename paramreader_t>
void Module<class_t>::register_params(paramreader_t&& paramIn)
{
  static_assert(Concepts::has_parameters<class_t>, "Module is missing VIPRA_REGISTER_PARAMS");

  Debug::debug_do([&]() { std::cout << "Registering Params For: " << self().module_name() << "\n"; });

  // get module parameters customization point
  auto params = self().parameters();

  // register all of the parameters with the parameter loader
  std::apply(
      [&](const auto& first, const auto&... restArgs) {
        auto regis = [&](const auto& param) {
          paramIn.register_param(self().module_type(), self().module_name(), param.first);
        };

        // recursively register each parameter
        regis(first);
        (regis(restArgs), ...);
      },
      params);
}

/**
   * @brief Loads in all parameters for the module
   * 
   * @tparam paramreader_t 
   * @param paramIn 
   * @param engine 
   */
template <typename class_t>
template <typename paramreader_t>
void Module<class_t>::config(paramreader_t& paramIn, VIPRA::Random::Engine& engine)
{
  static_assert(Concepts::has_parameters<class_t>, "Module is missing VIPRA_REGISTER_PARAMS");

  Debug::debug_do([&]() { std::cout << "Configuring Module: " << self().module_name() << "\n"; });

  // get module parameters customization point
  auto const& params = self().parameters();

  // load each parameter and apply it to the coresponding variable
  std::apply(
      [&](const auto& first, const auto&... restArgs) {
        auto configure = [&](const auto& param) {
          using param_t = std::remove_cvref_t<decltype(param.second)>;

          // set the parameter variable as loaded from input
          param.second = paramIn.template get_param<param_t>(self().module_type(), self().module_name(),
                                                             param.first, engine);
        };

        // recursively load the parameters
        configure(first);
        (configure(restArgs), ...);
      },
      params);
}
}  // namespace VIPRA::Modules