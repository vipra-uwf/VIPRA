#pragma once

#include <concepts>

#include "vipra/concepts/module.hpp"
#include "vipra/modules.hpp"

#include "vipra/geometry/f3d.hpp"
#include "vipra/random/random.hpp"
#include "vipra/types/idx.hpp"
#include "vipra/types/size.hpp"

namespace VIPRA {
struct State;
}

namespace VIPRA::Concepts {

// TODO(rolland): add getting distance between pedestrians
// TODO(rolland): add getting direction between pedestrians ?
// TODO(rolland): add getting nearest pedestrian that meets a condition ?

/**
 * @brief Checks that a type can get the number of pedestrians
 * 
 * @tparam pedset_t 
 */
template <typename pedset_t>
concept can_get_num_peds = requires(const pedset_t pedset) {
  { pedset.num_pedestrians() } -> std::same_as<VIPRA::size>;
};

/**
 * @brief Checks that a type can get pedestrian coordinates
 * 
 * @tparam pedset_t 
 */
template <typename pedset_t>
concept can_get_ped_coords = requires(const pedset_t pedset, VIPRA::idx idx) {
  { pedset.ped_coords(idx) } -> std::same_as<VIPRA::f3d const&>;
  { pedset.all_coords() } -> std::same_as<std::vector<VIPRA::f3d> const&>;
};

/**
 * @brief Checks that a type can get pedestrian velocity
 * 
 * @tparam pedset_t 
 */
template <typename pedset_t>
concept can_get_ped_velocity = requires(const pedset_t pedset, VIPRA::idx idx) {
  { pedset.ped_velocity(idx) } -> std::same_as<VIPRA::f3d const&>;
  { pedset.all_velocities() } -> std::same_as<std::vector<VIPRA::f3d> const&>;
};

/**
 * @brief Checks that a type can update the pedestrian set
 * 
 * @tparam pedset_t 
 */
template <typename pedset_t>
concept can_update_pedset = requires(pedset_t pedset, State const& state) {
  {pedset.update(state)};
};

/**
 * @brief Checks that a type is a pedestrian set module
 * 
 * @tparam pedset_t 
 */
template <typename pedset_t>
concept PedsetModule = is_module<pedset_t, VIPRA::Modules::Type::PEDESTRIANS> && can_get_num_peds<pedset_t> &&
    can_get_ped_coords<pedset_t> && can_get_ped_velocity<pedset_t> && can_update_pedset<pedset_t>;

/**
 * @brief Dummy pedestrian set for use in other concepts
 * 
 */
class DummyPedSet {
  // NOLINTBEGIN

 public:
  constexpr static VIPRA::Modules::Type _VIPRA_MODULE_TYPE_ = VIPRA::Modules::Type::PEDESTRIANS;

  template <typename params_t>
  void register_params(params_t&) {}

  void config(auto& params, VIPRA::Random::Engine&) {}
  void update(const VIPRA::State&) {}

  auto num_pedestrians() const -> VIPRA::size { return 1; }
  auto ped_coords(VIPRA::idx /*unused*/) const -> VIPRA::f3d const& { return _dummy2; }
  auto all_coords() const -> std::vector<VIPRA::f3d> const& { return _dummy; }
  auto ped_velocity(VIPRA::idx /*unused*/) const -> VIPRA::f3d const& { return _dummy2; }
  auto all_velocities() const -> std::vector<VIPRA::f3d> const& { return _dummy; }

 private:
  VIPRA::f3dVec _dummy;
  VIPRA::f3d    _dummy2;
  // NOLINTEND
};

CHECK_MODULE(PedsetModule, DummyPedSet);
}  // namespace VIPRA::Concepts