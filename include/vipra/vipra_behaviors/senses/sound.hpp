
#pragma once

#include <cstddef>

#include "vipra/geometry/f3d.hpp"

namespace VIPRA {
struct Sound {
  size_t     stimulus_id;
  VIPRA::f3d position;

  // TODO(rolland): create proper method without simple collisions
  static auto id(std::string_view name) noexcept -> size_t
  {
    size_t soundId = 0;

    for ( char chr : name ) {
      soundId += chr;
    }

    return soundId;
  }
};
}  // namespace VIPRA