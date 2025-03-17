#pragma once

#include <tuple>

namespace BADL {
template <typename base_t>
class BaseComponent {
 public:
  template <typename component_t>
  [[nodiscard]] auto get_component() const -> component_t const&
  {
    return std::get<component_t>(static_cast<base_t const*>(this)->_components);
  }

  template <typename component_t>
  [[nodiscard]] auto get_component() -> component_t&
  {
    return std::get<component_t>(static_cast<base_t*>(this)->_components);
  }
};

}  // namespace BADL