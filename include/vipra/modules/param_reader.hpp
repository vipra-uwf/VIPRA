#pragma once

#include <map>
#include <optional>
#include <random>
#include <type_traits>
#include <vector>

#include "vipra/concepts/numeric.hpp"

#include "vipra/random/random.hpp"
#include "vipra/util/crtp.hpp"

namespace VIPRA::Modules {

/**
 * @brief Base ParamReader Module
 * 
 */
template <typename module_t>
class ParamReader : public Util::CRTP<ParamReader<module_t>> {
 public:
  template <typename data_t>
  auto get_param(std::string const& type, std::string const& moduleName,
                 std::string const& paramName, VIPRA::Random::Engine& engine) const
      -> std::optional<std::remove_cvref_t<data_t>>
  {
    using base_data_t = std::remove_cvref_t<data_t>;

    auto singleValue =
        this->self().template get<base_data_t>({type, moduleName, paramName});
    if ( singleValue ) {
      return singleValue;
    }

    auto arrayValue = this->self().template get<std::vector<base_data_t>>(
        {type, moduleName, paramName});
    if ( arrayValue ) {
      return get_discrete_value<base_data_t>(arrayValue.value(), engine);
    }

    auto mapValue = this->self().template get<std::map<std::string, base_data_t>>(
        {type, moduleName, paramName});
    if ( mapValue ) {
      if constexpr ( Concepts::Numeric<base_data_t> ) {
        // NOTE(rolland): strings cannot be ranges
        return get_range_value<base_data_t>(mapValue.value(), engine);
      }
      else {
        return std::nullopt;
      }
    }

    return std::nullopt;
  }

 private:
  template <typename data_t>
  auto get_discrete_value(std::vector<data_t> const& data,
                          VIPRA::Random::Engine&     engine) const -> data_t
  {
    std::uniform_int_distribution<size_t> dist(0, data.size() - 1);
    return data[dist(engine)];
  }

  template <typename data_t>
    requires Concepts::Numeric<data_t>
  auto get_range_value(std::map<std::string, data_t> const& data,
                       VIPRA::Random::Engine&               engine) const -> data_t
  {
    if constexpr ( std::is_floating_point_v<data_t> ) {
      std::uniform_real_distribution<data_t> dist(data.at("min"), data.at("max"));
      return dist(engine);
    }
    else if constexpr ( std::is_integral_v<data_t> ) {
      std::uniform_int_distribution<data_t> dist(data.at("min"), data.at("max"));
      return dist(engine);
    }
    else {
      static_assert(false, "Invalid Type");
    }
  }
};

}  // namespace VIPRA::Modules