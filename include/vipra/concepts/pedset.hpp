#pragma once

#include <concepts>

#include "vipra/concepts/module.hpp"
#include "vipra/modules.hpp"

#include "vipra/types/f3d.hpp"
#include "vipra/types/idx.hpp"
#include "vipra/types/size.hpp"

namespace VIPRA::Concepts {

template <typename pedset_t>
concept PedsetModule = Module<pedset_t, VIPRA::Modules::Type::PEDESTRIANS> &&
    requires(const pedset_t pedset) {
  { pedset.num_pedestrians() } -> std::same_as<VIPRA::size>;
  { pedset.ped_coords(VIPRA::idx{}) } -> std::same_as<VIPRA::f3d>;
  { pedset.all_coords() } -> std::same_as<const std::vector<VIPRA::f3d>&>;
  { pedset.ped_velocity(VIPRA::idx{}) } -> std::same_as<VIPRA::f3d>;
  { pedset.all_velocities() } -> std::same_as<const std::vector<VIPRA::f3d>&>;
};

class DummyPedSet {
  // NOLINTBEGIN
  VIPRA_MODULE_TYPE(PEDESTRIANS);

 public:
  template <typename params_t>
  static void register_params() {}

  void setup(auto& params) {}

  auto num_pedestrians() const -> VIPRA::size { return 1; }
  auto ped_coords(VIPRA::idx /*unused*/) const -> VIPRA::f3d { return VIPRA::f3d{0}; }
  auto all_coords() const -> const std::vector<VIPRA::f3d>& { return _dummy; }
  auto ped_velocity(VIPRA::idx /*unused*/) const -> VIPRA::f3d { return VIPRA::f3d{0}; }
  auto all_velocities() const -> const std::vector<VIPRA::f3d>& { return _dummy; }

 private:
  VIPRA::f3dVec _dummy;
  DummyPedSet() = default;
  // NOLINTEND
};

CHECK_MODULE(PedsetModule, DummyPedSet);
}  // namespace VIPRA::Concepts