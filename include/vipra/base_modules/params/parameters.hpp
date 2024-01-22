#pragma once

#include <map>

#include <nlohmann/json.hpp>

#include "vipra/concepts/input.hpp"
#include "vipra/modules.hpp"
#include "vipra/types/parameter.hpp"

namespace VIPRA {
template <Concepts::InputModule input_t>
class Parameters {
  VIPRA_MODULE_TYPE(PARAMETERS)

 public:
  explicit Parameters(input_t&& input) : _input(input) {}

  void set_param(VIPRA::Modules::Type module, const std::string& name, Parameter param) {
    _params[module][name] = param;
  }

  template <typename data_t>
  auto get_param(VIPRA::Modules::Type module, const std::string& name) -> data_t {
    if (!_params[module].contains(name))
      throw std::runtime_error("Parameter: " + name + " For Module: " + to_string(module) +
                               " Not Registered");

    return _input.template get<data_t>(name);
  }

 private:
  std::map<VIPRA::Modules::Type, std::map<std::string, Parameter>> _params;
  input_t                                                          _input;
};
}  // namespace VIPRA