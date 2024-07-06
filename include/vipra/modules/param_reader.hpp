#pragma once

#include <map>
#include <optional>
#include <vector>

#include "vipra/concepts/numeric.hpp"
#include "vipra/modules/module.hpp"

#include "vipra/modules.hpp"

#include "vipra/random/distributions.hpp"
#include "vipra/random/random.hpp"

namespace VIPRA::Modules {

/**
 * @brief Dummy parameter input for use in other concepts
 * 
 */
template <typename module_t>
class ParamReader : public Module<ParamReader<module_t>> {
  // _VIPRA_MODULE_TYPE_, _VIPRA_MODULE_NAME_, paramName, engine
 public:
  template <typename data_t>
  auto get_param(VIPRA::Modules::Type type, std::string const& moduleName, std::string const& paramName,
                 VIPRA::Random::Engine& engine) const -> std::optional<data_t> {
    auto singleValue = this->self().template get<data_t>(to_string(type), moduleName, paramName);
    if ( singleValue ) {
      return singleValue;
    }

    auto arrayValue = this->self().template get<std::vector<data_t>>(to_string(type), moduleName, paramName);
    if ( arrayValue ) {
      return get_discrete_value<data_t>(arrayValue.value(), engine);
    }

    auto mapValue =
        this->self().template get<std::map<std::string, data_t>>(to_string(type), moduleName, paramName);
    if ( arrayValue ) {
      return get_range_value<data_t>(mapValue.value(), engine);
    }

    return std::nullopt;
  }

  // TODO(rolland): handle getting array parameters

 private:
  template <typename data_t>
  auto get_discrete_value(std::vector<data_t> const& data, VIPRA::Random::Engine& engine) -> data_t {
    VIPRA::Random::uniform_distribution<size_t> dist(0, data.size() - 1);
    return data[dist(engine)];
  }

  template <typename data_t>
  auto get_range_value(std::map<std::string, data_t> const& data, VIPRA::Random::Engine& engine) -> data_t {
    VIPRA::Random::uniform_distribution<data_t> dist(data.get()["min"], data.get()["max"]);
    return dist(engine);
  }
};

}  // namespace VIPRA::Modules
