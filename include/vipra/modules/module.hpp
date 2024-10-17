#pragma once

#include <tuple>
#include <type_traits>

#include "vipra/logging/logging.hpp"

#include "vipra/concepts/has_parameters.hpp"
#include "vipra/random/random.hpp"
#include "vipra/special_modules/parameters.hpp"
#include "vipra/util/crtp.hpp"

namespace VIPRA::Modules {

/**
 * @brief VIPRA Module Base CRTP Class
 * 
 * @tparam class_t 
 */
template <typename class_t>
class BaseModule : public Util::CRTP<BaseModule<class_t>> {
  using Util::CRTP<BaseModule<class_t>>::self;

 public:
  void register_base_params(Parameters& paramIn);

  void config_base(Parameters& paramIn, VIPRA::Random::Engine& engine);
};

/**
 * @brief VIPRA Module Base CRTP Class
 * 
 * @tparam class_t 
 */
template <typename class_t>
class Module : public Util::CRTP<Module<class_t>> {
  using Util::CRTP<Module<class_t>>::self;

 public:
  void register_params(Parameters& paramIn);

  void config(Parameters& paramIn, VIPRA::Random::Engine& engine);
};

// ----------------------------------------------------------------------------------------------------------------
// --------------------------------------- IMPLEMENTATIONS --------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------

/**
   * @brief Registers the modules parameters with the parameter reader
   * 
   * @tparam Parameters 
   * @param paramIn
   */
template <typename class_t>
void Module<class_t>::register_params(Parameters& paramIn)
{
  static_assert(Concepts::has_parameters<class_t>,
                "Module is missing VIPRA_REGISTER_PARAMS");

  VIPRA::Log::debug("Registering Params For: {}", self().module_name());

  // get module parameters customization point
  auto params = self().module_params();

  if constexpr ( ! std::is_same_v<std::tuple<>,
                                  std::remove_cvref_t<decltype(params)>> ) {
    // register all of the parameters with the parameter loader
    std::apply(
        [&](auto const& first, auto const&... restArgs) {
          auto regis = [&](auto const& param) {
            paramIn.register_param(self().module_type(), self().module_name(),
                                   param.first);
          };

          // recursively register each parameter
          regis(first);
          (regis(restArgs), ...);
        },
        params);
  }

  VIPRA::Log::debug("Done Registering Params For: {}", self().module_name());
}

/**
   * @brief Loads in all parameters for the module
   * 
   * @tparam Parameters 
   * @param paramIn 
   * @param engine 
   */
template <typename class_t>
void Module<class_t>::config(Parameters& paramIn, VIPRA::Random::Engine& engine)
{
  static_assert(Concepts::has_parameters<class_t>,
                "Module is missing VIPRA_REGISTER_PARAMS");

  VIPRA::Log::debug("Configuring Module: {}", self().module_name());

  // get module parameters customization point
  auto const& params = self().module_params();

  if constexpr ( ! std::is_same_v<std::tuple<>,
                                  std::remove_cvref_t<decltype(params)>> ) {
    // load each parameter and apply it to the coresponding variable
    std::apply(
        [&](auto const& first, auto const&... restArgs) {
          auto configure = [&](auto const& param) {
            using param_t = std::remove_cvref_t<decltype(param.second)>;
            VIPRA::Log::debug("Loading Param: {}, {}", param.first,
                              static_cast<void*>(&param.second));

            // set the parameter variable as loaded from input
            param.second = paramIn.get_param<param_t>(self().module_type(),
                                                      self().module_name(),
                                                      param.first, engine);
          };

          // recursively load the parameters
          configure(first);
          (configure(restArgs), ...);
        },
        params);
  }

  VIPRA::Log::debug("Done Configuring Module: {}", self().module_name());
}

/**
   * @brief Registers the modules parameters with the parameter reader
   * 
   * @tparam Parameters 
   * @param paramIn
   */
template <typename class_t>
void BaseModule<class_t>::register_base_params(Parameters& paramIn)
{
  // get module parameters customization point
  auto params = self().base_module_params();

  if constexpr ( ! std::is_same_v<std::tuple<>,
                                  std::remove_cvref_t<decltype(params)>> ) {
    // register all of the parameters with the parameter loader
    std::apply(
        [&](auto const& first, auto const&... restArgs) {
          auto regis = [&](auto const& param) {
            paramIn.register_param(self().module_type(), "Base", param.first);
          };

          // recursively register each parameter
          regis(first);
          (regis(restArgs), ...);
        },
        params);
  }
}

/**
   * @brief Loads in all parameters for the module
   * 
   * @tparam Parameters 
   * @param paramIn 
   * @param engine 
   */
template <typename class_t>
void BaseModule<class_t>::config_base(Parameters&            paramIn,
                                      VIPRA::Random::Engine& engine)
{
  // get module parameters customization point
  auto const& params = self().base_module_params();

  if constexpr ( ! std::is_same_v<std::tuple<>,
                                  std::remove_cvref_t<decltype(params)>> ) {
    // load each parameter and apply it to the coresponding variable
    std::apply(
        [&](auto const& first, auto const&... restArgs) {
          auto configure = [&](auto const& param) {
            using param_t = std::remove_cvref_t<decltype(param.second)>;
            VIPRA::Log::debug("Loading Param: {}, {}", param.first,
                              static_cast<void*>(&param.second));

            // set the parameter variable as loaded from input
            param.second = paramIn.get_param<param_t>(
                self().module_type(), "Base", param.first, engine);
          };

          // recursively load the parameters
          configure(first);
          (configure(restArgs), ...);
        },
        params);
  }
}
}  // namespace VIPRA::Modules