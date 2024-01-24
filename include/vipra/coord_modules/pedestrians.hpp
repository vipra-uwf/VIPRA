#pragma once

#include <tuple>

#include "vipra/concepts/input.hpp"
#include "vipra/concepts/pedset.hpp"

namespace VIPRA::Module {

template <Concepts::PedsetModule pedset_t>
class Pedestrians {
  VIPRA_MODULE_TYPE(PEDESTRIANS);

 public:
  explicit Pedestrians(pedset_t&& pedset) : _pedestrians(pedset) {}

  template <Concepts::ParamModule params_t>
  static void register_params() {}

  void setup(auto& params) {}

  [[nodiscard]] auto num_pedestrians() -> VIPRA::size { return _pedestrians.num_pedestrians(); }
  [[nodiscard]] auto ped_coords(VIPRA::idx pedIdx) -> VIPRA::f3d { return _pedestrians.ped_coords(pedIdx); }
  [[nodiscard]] auto all_coords() -> const std::vector<VIPRA::f3d>& { return _pedestrians.all_coords(); }
  [[nodiscard]] auto ped_velocity(VIPRA::idx pedIdx) -> VIPRA::f3d {
    return _pedestrians.ped_velocity(pedIdx);
  }
  [[nodiscard]] auto all_velocities() -> const std::vector<VIPRA::f3d>& {
    return _pedestrians.all_velocities();
  }

 private:
  pedset_t _pedestrians;
};

CHECK_MODULE(PedsetModule, Pedestrians<Concepts::DummyPedSet>);

}  // namespace VIPRA::Module