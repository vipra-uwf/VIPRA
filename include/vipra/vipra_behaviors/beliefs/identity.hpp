
#pragma once

#include <cstddef>

namespace VIPRA {
struct Identity {
  size_t idx;

  // NOLINTBEGIN(google-explicit-constructor)
  operator size_t() const { return idx; }
  Identity(size_t const& index) : idx(index) {}
  Identity(size_t&& index) : idx(index) {}
  // NOLINTEND(google-explicit-constructor)

  Identity() = default;
  Identity(const Identity&) = default;
  Identity(Identity&&) noexcept = default;
  auto operator=(const Identity&) -> Identity& = default;
  auto operator=(Identity&&) noexcept -> Identity& = default;
  ~Identity() = default;
};
}  // namespace VIPRA