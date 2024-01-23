#pragma once

#include <map>

#include <nlohmann/json.hpp>

#include "vipra/concepts/input.hpp"
#include "vipra/modules.hpp"
#include "vipra/types/parameter.hpp"

// TODO(rolland): Check that all required parameters are provided
namespace VIPRA {
template <Concepts::InputModule input_t>
class Parameters {
  VIPRA_MODULE_TYPE(PARAMETERS)

 public:
  explicit Parameters<input_t>(input_t&& input) : _input(input) {}

  static void register_param(VIPRA::Modules::Type module, const std::string& name, Parameter param) {
    get_params()[module][name] = param;
  }

  template <typename data_t>
  auto get_param(VIPRA::Modules::Type module, const std::string& name) -> data_t {
    if (!get_params()[module].contains(name))
      throw std::runtime_error("Parameter: " + name + " For Module: " + to_string(module) +
                               " Not Registered");

    auto value = _input.template get<data_t>(name);
    if (!value.has_value()) {
      if (get_params()[module][name].type == Parameter::Type::REQUIRED)
        throw std::runtime_error("Required Parameter: " + name + " For Module: " + to_string(module) +
                                 " Not Provided In Input");
    }

    return value.value();
  }

 private:
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables) Bug in Clang-Tidy

  input_t _input;

  static auto get_params() -> std::map<VIPRA::Modules::Type, std::map<std::string, Parameter>>& {
    static std::map<VIPRA::Modules::Type, std::map<std::string, Parameter>> params{};
    return params;
  }
};
}  // namespace VIPRA