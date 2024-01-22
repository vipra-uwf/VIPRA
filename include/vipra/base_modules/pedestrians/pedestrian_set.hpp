#pragma once

#include <cstdio>

#include "vipra/concepts/input.hpp"
#include "vipra/modules.hpp"

#include "vipra/types/f3d.hpp"
#include "vipra/types/idx.hpp"
#include "vipra/types/size.hpp"

namespace VIPRA {
class Pedestrians {
  VIPRA_MODULE_TYPE(PEDESTRIANS)

 public:
  template <Concepts::InputModule input_t>
  explicit Pedestrians(const input_t& input) {}

  template <typename params_t>
  void set_params(params_t& params) {}

  [[nodiscard]] auto num_pedestrians() -> VIPRA::size { return _coords.size(); }
  [[nodiscard]] auto ped_coords(VIPRA::idx pedIdx) -> VIPRA::f3d { return _coords[pedIdx]; }
  [[nodiscard]] auto all_coords() -> const std::vector<VIPRA::f3d>& { return _coords; }
  [[nodiscard]] auto ped_velocity(VIPRA::idx pedIdx) -> VIPRA::f3d { return _velocities[pedIdx]; }
  [[nodiscard]] auto all_velocities() -> const std::vector<VIPRA::f3d>& { return _velocities; }

 private:
  VIPRA::f3dVec _coords;
  VIPRA::f3dVec _velocities;
};
}  // namespace VIPRA