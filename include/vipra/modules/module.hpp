#pragma once

#include <concepts>
#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>

#include "vipra/concepts/has_parameters.hpp"
#include "vipra/modules.hpp"
#include "vipra/random/random.hpp"
#include "vipra/util/crtp.hpp"

namespace VIPRA::Modules {

template <typename class_t>
class Module : public Util::CRTP<Module<class_t>> {
  using Util::CRTP<Module<class_t>>::self;

 public:
  /**
   * @brief Registers the modules parameters with the parameter reader
   * 
   * @tparam paramreader_t 
   * @param paramIn
   */
  template <typename paramreader_t>
  void register_params(paramreader_t&& paramIn) {
    static_assert(Concepts::has_parameters<class_t>, "Module is missing VIPRA_REGISTER_PARAMS");

    // get module parameters customization point
    auto const& params = self().parameters();

    // register all of the parameters with the parameter loader
    std::apply(
        [&](auto const& param) {
          paramIn.register_param(self().module_type(), self().module_name(), param.first);
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
  template <typename paramreader_t>
  void config(paramreader_t& paramIn, VIPRA::Random::Engine& engine) {
    static_assert(Concepts::has_parameters<class_t>, "Module is missing VIPRA_REGISTER_PARAMS");

    // get module parameters customization point
    auto const& params = self().parameters();

    // load the parameter and apply it to the coresponding variable
    std::apply(
        [&](auto const& param) {
          param.second = paramIn.template get_param<std::remove_cv_t<decltype(param.second)>>(
              self().module_type(), self().module_name(), param.first, engine);
        },
        params);
  }
};
}  // namespace VIPRA::Modules