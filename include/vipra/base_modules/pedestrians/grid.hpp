#pragma once

#include <iostream>

#include "vipra/concepts/input.hpp"
#include "vipra/concepts/pedset.hpp"
#include "vipra/macros/module.hpp"
#include "vipra/modules.hpp"

#include "vipra/geometry/f3d.hpp"
#include "vipra/types/idx.hpp"
#include "vipra/types/size.hpp"
#include "vipra/types/state.hpp"

#include "vipra/util/debug_do.hpp"

namespace VIPRA::Pedestrians {
template <Concepts::InputModule input_t>
class Grid {
 public:
  VIPRA_MODULE_TYPE(PEDESTRIANS);

  explicit Grid(input_t&& input) : _input(input) {
    auto coords = input.template get<std::vector<VIPRA::f3d>>("coords");
    if (!coords) throw std::runtime_error("Could not find pedestrian coordinates in input file");

    _velocities = std::vector<VIPRA::f3d>((*coords).size());
    _coords = std::move(*coords);
  }

  void update(const VIPRA::State& state) {
    assert(state.size() == _coords.size());

    for (VIPRA::idx pedIdx = 0; pedIdx < state.size(); ++pedIdx) {
      _coords[pedIdx] = state.positions[pedIdx];
      _velocities[pedIdx] = state.velocities[pedIdx];
    }
  }

  template <Concepts::ParamModule params_t>
  void register_params(params_t& params) {}

  void config(auto const& params) {}

  [[nodiscard]] auto num_pedestrians() const -> VIPRA::size { return _coords.size(); }
  [[nodiscard]] auto ped_coords(VIPRA::idx pedIdx) const -> VIPRA::f3d const& {
    assert(pedIdx < _coords.size());

    return _coords[pedIdx];
  }
  [[nodiscard]] auto all_coords() const -> std::vector<VIPRA::f3d> const& { return _coords; }
  [[nodiscard]] auto ped_velocity(VIPRA::idx pedIdx) const -> VIPRA::f3d const& {
    assert(pedIdx < _velocities.size());

    return _velocities[pedIdx];
  }
  [[nodiscard]] auto all_velocities() const -> std::vector<VIPRA::f3d> const& { return _velocities; }

 private:
  VIPRA::f3dVec _coords;
  VIPRA::f3dVec _velocities;

  input_t _input;
};
}  // namespace VIPRA::Pedestrians

CHECK_MODULE(PedsetModule, VIPRA::Pedestrians::Grid<VIPRA::Concepts::DummyInput>)