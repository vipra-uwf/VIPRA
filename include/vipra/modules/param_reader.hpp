#pragma once

#include <map>
#include <optional>
#include <type_traits>
#include <vector>

#include "vipra/concepts/numeric.hpp"
#include "vipra/modules/module.hpp"

#include "vipra/modules.hpp"

#include "vipra/random/distributions.hpp"
#include "vipra/random/random.hpp"
#include "vipra/util/crtp.hpp"

namespace VIPRA::Modules {

/**
 * @brief Dummy parameter input for use in other concepts
 * 
 */
template <typename module_t>
class ParamReader : public Util::CRTP<ParamReader<module_t>> {
  // _VIPRA_MODULE_TYPE_, _VIPRA_MODULE_NAME_, paramName, engine
 public:
  template <typename data_t>
  auto get_param(std::string const& type, std::string const& moduleName, std::string const& paramName,
                 VIPRA::Random::Engine& engine) const -> std::optional<std::remove_cvref_t<data_t>> {
    auto singleValue = this->self().template get<std::remove_cvref_t<data_t>>(type, moduleName, paramName);
    if ( singleValue ) {
      return singleValue;
    }

    auto arrayValue =
        this->self().template get<std::vector<std::remove_cvref_t<data_t>>>(type, moduleName, paramName);
    if ( arrayValue ) {
      return get_discrete_value<std::remove_cvref_t<data_t>>(arrayValue.value(), engine);
    }

    auto mapValue = this->self().template get<std::map<std::string, std::remove_cvref_t<data_t>>>(
        type, moduleName, paramName);
    if ( mapValue ) {
      if constexpr ( Concepts::Numeric<std::remove_cvref_t<data_t>> ) {
        // NOTE(rolland): strings cannot be ranges
        return get_range_value<std::remove_cvref_t<data_t>>(mapValue.value(), engine);
      } else {
        return std::nullopt;
      }
    }

    return std::nullopt;
  }

 private:
  template <typename data_t>
  auto get_discrete_value(std::vector<data_t> const& data, VIPRA::Random::Engine& engine) const -> data_t {
    VIPRA::Random::uniform_distribution<size_t> dist(0, data.size() - 1);
    return data[dist(engine)];
  }

  template <typename data_t>
  requires Concepts::Numeric<data_t>
  auto get_range_value(std::map<std::string, data_t> const& data, VIPRA::Random::Engine& engine) const
      -> data_t {
    VIPRA::Random::uniform_distribution<data_t> dist(data.at("min"), data.at("max"));
    return dist(engine);
  }
};

}  // namespace VIPRA::Modules