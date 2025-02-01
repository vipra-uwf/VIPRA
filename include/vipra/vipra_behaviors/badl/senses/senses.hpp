#pragma once

#include "badl/components/component.hpp"

namespace BADL {
template <typename... sense_ts>
class Senses : public BADL::Component {
 public:
  [[nodiscard]] auto get_query_grammar() const
      -> std::vector<
          std::tuple<std::string, std::string_view, QueryFunc>> override
  {
    std::vector<std::tuple<std::string, std::string_view, QueryFunc>> results;
    std::apply(
        [&](auto const&... actuator) {
          const auto append = [&](auto const& curr_actuator) {
            auto grammar = curr_actuator.get_query_grammar();
            results.insert(results.end(), grammar.begin(), grammar.end());
          };

          (append(actuator), ...);
        },
        _senses);

    return results;
  };

  [[nodiscard]] auto get_activation_grammar() const
      -> std::vector<
          std::tuple<std::string, std::string_view, ActivationFunc>> override
  {
    std::vector<std::tuple<std::string, std::string_view, ActivationFunc>>
        results;
    std::apply(
        [&](auto const&... actuator) {
          const auto append = [&](auto const& curr_actuator) {
            auto grammar = curr_actuator.get_activation_grammar();
            results.insert(results.end(), grammar.begin(), grammar.end());
          };

          (append(actuator), ...);
        },
        _senses);

    return results;
  }

 private:
  std::tuple<sense_ts...> _senses;
};
}  // namespace BADL