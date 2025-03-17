#pragma once

#include <string_view>
#include <utility>
#include <vector>

#include "badl/components/component_macros.hpp"
#include "badl/components/component_methods.hpp"

namespace BADL {

class Component {
 public:
  [[nodiscard]] virtual auto get_query_grammar() const
      -> std::vector<std::tuple<std::string, std::string_view, QueryFunc>>
  {
    return {};
  };

  [[nodiscard]] virtual auto get_activation_grammar() const
      -> std::vector<std::tuple<std::string, std::string_view, ActivationFunc>>
  {
    return {};
  }

  Component(const Component&) = default;
  Component(Component&&) = default;
  auto operator=(const Component&) -> Component& = default;
  auto operator=(Component&&) -> Component& = default;
  Component() = default;
  virtual ~Component() = default;
};
}  // namespace BADL