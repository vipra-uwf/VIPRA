#pragma once

#include <concepts>
#include <cstddef>
#include <tuple>
#include <utility>

#include "vipra/modules.hpp"
#include "vipra/random/random.hpp"
#include "vipra/util/crtp.hpp"

namespace VIPRA::Modules {

template <typename class_t>
class Module : public Util::CRTP<Module<class_t>> {
 public:
  /**
   * @brief Registers the modules parameters with the parameter reader
   * 
   * @tparam paramreader_t 
   * @param paramIn
   */
  template <typename paramreader_t>
  void register_params(paramreader_t& paramIn) {
    auto const& params = this->self().parameters();

    std::apply(
        [&](auto const& param) {
          paramIn.register_param(this->self().module_type(), this->self().module_name(), param.first);
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
    auto const& params = this->self().parameters();

    std::apply(
        [&](auto const& param) {
          param.second = paramIn.template get_param<std::remove_cv_t<decltype(param.second)>>(
              this->self().module_type(), this->self().module_name(), param.first, engine);
        },
        params);
  }
};
}  // namespace VIPRA::Modules