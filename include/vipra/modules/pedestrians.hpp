#pragma once

#include <concepts>
#include <type_traits>
#include <vector>

#include "vipra/modules/module.hpp"

#include "vipra/geometry/f3d.hpp"

#include "vipra/random/random.hpp"

#include "vipra/types/float.hpp"
#include "vipra/types/idx.hpp"
#include "vipra/types/size.hpp"
#include "vipra/types/state.hpp"
#include "vipra/types/util/result_or_void.hpp"
#include "vipra/util/crtp.hpp"

namespace VIPRA::Modules {

template <typename func_t>
concept Condition = std::is_invocable_r<bool, func_t, VIPRA::idx>::value || std::is_same_v<func_t, VOID>;

// TODO(rolland): add getting direction between pedestrians ?

/**
 * @brief Dummy pedestrian set for use in other concepts
 * 
 */
template <typename module_t>
class Pedestrians : public Util::CRTP<Pedestrians<module_t>> {
  using Util::CRTP<Pedestrians<module_t>>::self;

 public:
  void initialize(auto& input, auto const& obstacles) { self().init_step(input, obstacles); }

  void update(VIPRA::State const& state)
  {
    self().update_step(state);

    for ( VIPRA::idx pedIdx = 0; pedIdx < state.size(); ++pedIdx ) {
      _coords[pedIdx] = state.positions[pedIdx];
      _velocities[pedIdx] = state.velocities[pedIdx];
    }
  }

  [[nodiscard]] auto all_neighbors_within(VIPRA::idx   pedIdx,
                                          VIPRA::f_pnt radius) const -> std::vector<VIPRA::idx>
  {
    return self().all_neighbors_within_impl(pedIdx, radius);
  }

  template <Condition condition_t>
  [[nodiscard]] auto closest_ped(VIPRA::idx pedIdx, condition_t&& condition = VOID{}) const -> VIPRA::idx
  {
    if constexpr ( std::is_same_v<condition_t, VOID> ) {
      return self().closest_ped_impl(pedIdx);
    }
    else {
      return self().conditional_closest_ped_impl(pedIdx, std::forward<condition_t>(condition));
    }
  }

  [[nodiscard]] auto distance(VIPRA::idx firstPed, VIPRA::idx secondPed) const -> VIPRA::f_pnt
  {
    return _coords[firstPed].distance_to(_coords[secondPed]);
  }

  [[nodiscard]] auto num_pedestrians() const -> VIPRA::size { return _coords.size(); }

  [[nodiscard]] auto ped_coords(VIPRA::idx pedIdx) const -> VIPRA::f3d const&
  {
    assert(pedIdx < _coords.size());

    return _coords[pedIdx];
  }

  [[nodiscard]] auto all_coords() const -> std::vector<VIPRA::f3d> const& { return _coords; }

  [[nodiscard]] auto ped_velocity(VIPRA::idx pedIdx) const -> VIPRA::f3d const&
  {
    assert(pedIdx < _velocities.size());
    return _velocities[pedIdx];
  }

  [[nodiscard]] auto all_velocities() const -> std::vector<VIPRA::f3d> const& { return _velocities; }

 private:
  VIPRA::f3dVec _coords;
  VIPRA::f3dVec _velocities;

 protected:
  void set_velocities(VIPRA::f3dVec const& velocities) { _velocities = velocities; }
  void set_velocities(VIPRA::f3dVec&& velocities) noexcept { _velocities = velocities; }

  void set_coordinates(VIPRA::f3dVec const& coordinates) { _coords = coordinates; }
  void set_coordinates(VIPRA::f3dVec&& coordinates) noexcept { _coords = coordinates; }

  [[nodiscard]] auto get_velocities() const -> VIPRA::f3dVec const& { return _velocities; }
  [[nodiscard]] auto get_coordinates() const -> VIPRA::f3dVec const& { return _coords; }
};

}  // namespace VIPRA::Modules