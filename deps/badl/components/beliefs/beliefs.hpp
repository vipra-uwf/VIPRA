
#pragma once

#include <tuple>
#include "badl/components/component.hpp"

namespace BADL {
template <typename... belief_ts>
class Beliefs : public BADL::Component {
 public:
  [[nodiscard]] auto get_query_grammar() const
      -> std::vector<
          std::tuple<std::string, std::string_view, QueryFunc>> override
  {
    // TODO(rolland): add
    return {};
  };

  [[nodiscard]] auto get_activation_grammar() const
      -> std::vector<
          std::tuple<std::string, std::string_view, ActivationFunc>> override
  {
    // TODO(rolland): add
    return {};
  }

  template <typename belief_t>
  auto get() const noexcept -> belief_t const&
  {
    return std::get<belief_t>(_beliefs);
  }
  template <typename belief_t>
  auto get() noexcept -> belief_t&
  {
    return std::get<belief_t>(_beliefs);
  }

 private:
  std::tuple<belief_ts...> _beliefs;
};
}  // namespace BADL