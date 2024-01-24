#pragma once

#include <vector>
#include "vipra/types/f3d.hpp"

namespace VIPRA {
/**
  * @brief Struct that holds the updated positions and velocities of all pedestrians
  * 
  */
struct State {
  std::vector<f3d> positions;
  std::vector<f3d> velocities;

  void initialize(size_t size) {
    positions.resize(size);
    velocities.resize(size);
  }
};
}  // namespace VIPRA