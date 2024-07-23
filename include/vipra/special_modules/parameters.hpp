#pragma once

#include <iostream>
#include <map>

#include <nlohmann/json.hpp>
#include <set>
#include <type_traits>

#include "vipra/debug/debug_do.hpp"
#include "vipra/macros/module.hpp"
#include "vipra/modules.hpp"
#include "vipra/random/distributions.hpp"
#include "vipra/random/random.hpp"
#include "vipra/types/parameter.hpp"
#include "vipra/util/template_specialization.hpp"

// TODO(rolland): Check that all required parameters are provided (maybe not needed, they are checked when the module tries to get it)
// TODO(rolland): issue #26 No way of checking what a parameter was, once get_param is called the random engine moves on
//                  - this is needed for outputing the parameters used in a simulation run

namespace VIPRA {
template <typename input_t>
class Parameters {
 public:
  VIPRA_MODULE_TYPE(PARAMETERS)

  explicit Parameters<input_t>(input_t&& input) : _input(input) {}

  /**
   * @brief Registers a parameter for a module
   * 
   * @tparam module_t 
   * @tparam param_t 
   * @param module 
   * @param moduleName 
   * @param paramName 
   * @param param 
   */
  void register_param(Modules::Type module, std::string const& moduleName, std::string const& paramName)
  {
    // TODO(rolland): maybe warn if a parameter is registered twice?
    if ( contains(module, moduleName, paramName) ) return;

    _params[module][moduleName].insert(paramName);
  }

  /**
   * @brief Returns true if the parameter exists, otherwise false
   * 
   * @param module 
   * @param moduleName 
   * @param paramName 
   * @return true 
   * @return false 
   */
  [[nodiscard]] auto has_param(Modules::Type module, std::string const& moduleName,
                               std::string const& paramName) const -> bool
  {
    return contains(module, moduleName, paramName);
  }

  /**
   * @brief Returns true if the parameter exists, otherwise false
   * 
   * @param module 
   * @param moduleName 
   * @param paramName 
   * @return true 
   * @return false 
   */
  [[nodiscard]] auto has_required_param(Modules::Type module, std::string const& moduleName,
                                        std::string const& paramName) const -> bool
  {
    if ( ! contains(module, moduleName, paramName) ) {
      throw std::runtime_error("Parameter: " + paramName + " For " + to_string(module) +
                               " Module: " + moduleName + " Not Provided In Input");
    }
    return true;
  }

  /**
   * @brief Returns the value of the parameter if it exists, otherwise throws an error
   * 
   * @tparam data_t 
   * @param module 
   * @param moduleName 
   * @param paramName 
   * @return data_t 
   */
  template <typename data_t>
  [[nodiscard]] auto get_param(Modules::Type module, std::string const& moduleName,
                               std::string const& paramName, Random::Engine& engine) const
      -> std::remove_cvref_t<data_t>
  {
    using param_t = std::remove_cvref_t<data_t>;

    std::string moduleStr = to_string(module);

    // Check that the parameter was registered
    if ( ! contains(module, moduleName, paramName) ) {
      throw std::runtime_error("Parameter: " + paramName + " For " + to_string(module) +
                               " Module: " + moduleName + " Not Registered");
    }

    // load the value of the parameter
    auto value = std::optional<param_t>{};
    if constexpr ( Util::is_specialization<param_t, std::vector>::value ) {
      // the parameter is supposed to be an array, load it as such
      value = get_array_param<param_t>(module, moduleName, paramName);
    }
    else {
      // the parameter is meant to be a single value, follow parameter randomization rules
      value = _input.template get_param<param_t>(moduleStr, moduleName, paramName, engine);
    }

    if ( ! value.has_value() ) {
      // parameter value wasn't found, throw
      throw std::runtime_error("Required Parameter: " + paramName + " For " + to_string(module) +
                               " Module: " + moduleName + " Not Provided In Input");
    }

    // Debug::debug_do([&]() {
    //   std::cout << "Parameter: " << paramName << " For " << to_string(module) << " Module: " << moduleName
    //             << " Value: " << value.value() << std::endl;
    // });

    return param_t{std::move(value.value())};
  }

  [[nodiscard]] auto get_input() -> input_t& { return _input; }

 private:
  input_t                                                               _input;
  std::map<Modules::Type, std::map<std::string, std::set<std::string>>> _params;

  [[nodiscard]] auto contains(Modules::Type module, std::string const& moduleName,
                              std::string const& paramName) const -> bool
  {
    return _params.contains(module) && _params.at(module).contains(moduleName) &&
           _params.at(module).at(moduleName).contains(paramName);
  }

  /**
   * @brief Gets an array parameter from the input
   * @note Arrays are generally used for random discrete values, however this method is for a parameter that is required to be an array
   *
   * @tparam array_t 
   * @param module 
   * @param moduleName 
   * @param paramName 
   * @return array_t 
   */
  template <typename array_t>
  [[nodiscard]] auto get_array_param(Modules::Type module, std::string const& moduleName,
                                     std::string const& paramName) const -> array_t
  {
    std::string moduleStr = to_string(module);

    if ( ! contains(module, moduleName, paramName) )
      throw std::runtime_error("Parameter: " + paramName + " For " + to_string(module) +
                               " Module: " + moduleName + " Not Registered");

    auto value = _input.template get<array_t>(moduleStr, moduleName, paramName);
    if ( ! value.has_value() ) {
      throw std::runtime_error("Required Parameter: " + paramName + " For " + to_string(module) +
                               " Module: " + moduleName + " Not Provided In Input");
    }

    return value.value();
  }
};

}  // namespace VIPRA