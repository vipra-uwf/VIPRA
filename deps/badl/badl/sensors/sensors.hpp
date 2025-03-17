#pragma once

#include <string>
#include <tuple>
#include <vector>

#include "badl/components/base_component.hpp"
#include "badl/components/component.hpp"

namespace BADL {
template <typename... sensor_ts>
class Sensors : public BADL::Component,
                public BADL::BaseComponent<Sensors<sensor_ts...>> {
 public:
  [[nodiscard]] auto get_query_grammar() const
      -> std::vector<
          std::tuple<std::string, std::string_view, QueryFunc>> override
  {
    std::vector<std::tuple<std::string, std::string_view, QueryFunc>> results;

    if constexpr ( std::tuple_size_v<decltype(_components)> != 0 )
      std::apply(
          [&](auto const&... actuator) {
            const auto append = [&](auto const& curr_actuator) {
              auto grammar = curr_actuator.get_query_grammar();
              results.insert(results.end(), grammar.begin(), grammar.end());
            };

            (append(actuator), ...);
          },
          _components);

    return results;
  };

  [[nodiscard]] auto get_activation_grammar() const
      -> std::vector<
          std::tuple<std::string, std::string_view, ActivationFunc>> override
  {
    std::vector<std::tuple<std::string, std::string_view, ActivationFunc>>
        results;

    if constexpr ( std::tuple_size_v<decltype(_components)> != 0 )
      std::apply(
          [&](auto const&... actuator) {
            const auto append = [&](auto const& curr_actuator) {
              auto grammar = curr_actuator.get_activation_grammar();
              results.insert(results.end(), grammar.begin(), grammar.end());
            };

            (append(actuator), ...);
          },
          _components);

    return results;
  }

 private:
  std::tuple<sensor_ts...> _components;
};
}  // namespace BADL