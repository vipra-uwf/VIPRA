#pragma once

#include <iostream>

#include "vipra/concepts/input.hpp"
#include "vipra/concepts/pedset.hpp"
#include "vipra/modules.hpp"

#include "vipra/types/f3d.hpp"
#include "vipra/types/idx.hpp"
#include "vipra/types/size.hpp"

namespace VIPRA::Pedestrians {
class Grid {
  VIPRA_MODULE_TYPE(PEDESTRIANS)

 public:
  template <Concepts::InputModule input_t>
  explicit Grid(const input_t& input) {
    auto coords = input.template get_vector<VIPRA::f3d>("coords");
    if (!coords) throw std::runtime_error("Could not find pedestrian coordinates in input file");

    _velocities = std::vector<VIPRA::f3d>((*coords).size());
    _coords = std::move(*coords);
  }

  template <Concepts::ParamModule params_t>
  static void register_params() {}

  void setup(auto& params) {}

  [[nodiscard]] auto num_pedestrians() const -> VIPRA::size { return _coords.size(); }
  [[nodiscard]] auto ped_coords(VIPRA::idx pedIdx) const -> VIPRA::f3d { return _coords[pedIdx]; }
  [[nodiscard]] auto all_coords() const -> const std::vector<VIPRA::f3d>& { return _coords; }
  [[nodiscard]] auto ped_velocity(VIPRA::idx pedIdx) const -> VIPRA::f3d { return _velocities[pedIdx]; }
  [[nodiscard]] auto all_velocities() const -> const std::vector<VIPRA::f3d>& { return _velocities; }

 private:
  VIPRA::f3dVec _coords;
  VIPRA::f3dVec _velocities;
};
}  // namespace VIPRA::Pedestrians

CHECK_MODULE(PedsetModule, VIPRA::Pedestrians::Grid)