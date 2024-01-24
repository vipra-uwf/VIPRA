#pragma once

#include <map>

#include <nlohmann/json.hpp>

#include "vipra/concepts/input.hpp"
#include "vipra/concepts/parameter_input.hpp"
#include "vipra/concepts/parameters.hpp"
#include "vipra/modules.hpp"
#include "vipra/types/parameter.hpp"

// TODO(rolland): Check that all required parameters are provided
namespace VIPRA {
template <Concepts::parameter_qualified_input input_t>
class Parameters {
  VIPRA_MODULE_TYPE(PARAMETERS)

 public:
  explicit Parameters<input_t>(input_t&& input) : _input(input) {}

  static void register_param(VIPRA::Modules::Type module, const std::string& name, ParameterType param) {
    get_params()[module][name] = param;
  }

  template <typename data_t>
  [[nodiscard]] auto get_param(VIPRA::Modules::Type module, const std::string& name) const -> data_t {
    std::string moduleStr = to_string(module);
    std::transform(moduleStr.begin(), moduleStr.end(), moduleStr.begin(), ::tolower);

    if (!get_params()[module].contains(name))
      throw std::runtime_error("Parameter: " + name + " For Module: " + to_string(module) +
                               " Not Registered");

    auto value = _input.template get<Parameter<data_t>>(moduleStr, name);
    if (!value.has_value()) {
      if (get_params()[module][name] == ParameterType::REQUIRED)
        throw std::runtime_error("Required Parameter: " + name + " For Module: " + to_string(module) +
                                 " Not Provided In Input");
    }

    return randomize_parameter(value.value());
  }

 private:
  input_t _input;

  static auto get_params() -> std::map<VIPRA::Modules::Type, std::map<std::string, ParameterType>>& {
    static std::map<VIPRA::Modules::Type, std::map<std::string, ParameterType>> params{};
    return params;
  }

  [[nodiscard]] constexpr auto randomize_parameter(auto&& paramVal) const {
    // TODO(rolland): randomize parameter, currently just pass through
    return paramVal.value;
  }
};

CHECK_MODULE(ParamModule, Parameters<Concepts::DummyInput>)

}  // namespace VIPRA