#pragma once

#include "vipra/geometry/f3d.hpp"

namespace VIPRA {
struct Position {
  VIPRA::f3d position;

  // NOLINTBEGIN(google-explicit-constructor)
  operator VIPRA::f3d() const { return position; }
  Position(VIPRA::f3d const& pos) : position(pos) {}
  Position(VIPRA::f3d&& pos) : position(pos) {}
  // NOLINTEND(google-explicit-constructor)

  Position() = default;
  Position(const Position&) = default;
  Position(Position&&) noexcept = default;
  auto operator=(const Position&) -> Position& = default;
  auto operator=(Position&&) noexcept -> Position& = default;
  ~Position() = default;
};
}  // namespace VIPRA