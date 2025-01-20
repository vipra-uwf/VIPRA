
#pragma once

#include <tuple>

namespace BADL {
template <typename... belief_ts>
class Beliefs {
 public:
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