

#include "vipra/special_modules/parameters.hpp"

namespace VIPRA {

/**
 * @brief Clears the parameters, does NOT reset the input module
 * 
 * @tparam VIPRA::Input::JSON 
 */
void Parameters::reset()
{
  _usedParameters.clear();
  _params.clear();
}

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
void Parameters::register_param(Modules::Type      module,
                                std::string const& moduleName,
                                std::string const& paramName)
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
auto Parameters::has_param(Modules::Type module, std::string const& moduleName,
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
auto Parameters::has_required_param(Modules::Type      module,
                                    std::string const& moduleName,
                                    std::string const& paramName) const -> bool
{
  if ( ! contains(module, moduleName, paramName) ) {
    throw std::runtime_error("Parameter: " + paramName + " For " +
                             to_string(module) + " Module: " + moduleName +
                             " Not Provided In Input");
  }
  return true;
}

}  // namespace VIPRA