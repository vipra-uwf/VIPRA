#pragma once

#include <functional>

#include "vipra/geometry/f3d.hpp"
#include "vipra/macros/pedestrians.hpp"
#include "vipra/modules/pedestrians.hpp"
#include "vipra/types/float.hpp"
#include "vipra/types/idx.hpp"
#include "vipra/types/size.hpp"
#include "vipra/types/util/result_or_void.hpp"

namespace VIPRA::Views {
class Pedestrians : public Modules::Pedestrians<Pedestrians> {
  using dist_func_t = VIPRA::f_pnt(void const*, VIPRA::idx, VIPRA::idx);
  using numped_func_t = VIPRA::size(void const*);
  using pedcoords_func_t = VIPRA::f3d const&(void const*, VIPRA::idx);
  using allcoords_func_t = std::vector<VIPRA::f3d> const&(void const*);
  using pedveloc_func_t = VIPRA::f3d const&(void const*, VIPRA::idx);
  using allvelocs_func_t = std::vector<VIPRA::f3d> const&(void const*);
  using closest_func_t = VIPRA::idx(void const*, VIPRA::idx);
  using condclosest_func_t = VIPRA::idx(void const*, VIPRA::idx, std::function<bool(VIPRA::idx)> const&);
  using allneighborswithin_func_t = std::vector<VIPRA::idx>(void const*, VIPRA::idx pedIdx,
                                                            VIPRA::f_pnt radius);

 private:
  void const* _module;

  dist_func_t*               _dist;
  numped_func_t*             _numpeds;
  pedcoords_func_t*          _pedcoords;
  allcoords_func_t*          _allcoords;
  pedveloc_func_t*           _pedveloc;
  allvelocs_func_t*          _allveloc;
  closest_func_t*            _closestped;
  condclosest_func_t*        _condclosest;
  allneighborswithin_func_t* _allneighborswithin;

 public:
  template <typename module_t>
  // NOLINTNEXTLINE(google-explicit-constructor)
  Pedestrians(module_t const& module) : _module(&module)
  {
    // NOLINTBEGIN(cppcoreguidelines-prefer-member-initializer)
    _dist = +[](void const* ptr, VIPRA::idx first, VIPRA::idx second) {
      return static_cast<module_t const*>(ptr)->distance(first, second);
    };

    _numpeds = +[](void const* ptr) { return static_cast<module_t const*>(ptr)->num_pedestrians(); };

    _pedcoords = +[](void const* ptr, VIPRA::idx pedIdx) -> VIPRA::f3d const& {
      return static_cast<module_t const*>(ptr)->ped_coords(pedIdx);
    };

    _allcoords = +[](void const* ptr) -> VIPRA::f3dVec const& {
      return static_cast<module_t const*>(ptr)->all_coords();
    };

    _pedveloc = +[](void const* ptr, VIPRA::idx pedIdx) -> VIPRA::f3d const& {
      return static_cast<module_t const*>(ptr)->ped_velocity(pedIdx);
    };

    _allveloc = +[](void const* ptr) -> VIPRA::f3dVec const& {
      return static_cast<module_t const*>(ptr)->all_velocities();
    };

    _closestped = +[](void const* ptr, VIPRA::idx pedIdx) {
      return static_cast<module_t const*>(ptr)->closest_ped_impl(pedIdx);
    };

    _condclosest = +[](void const* ptr, VIPRA::idx pedIdx, std::function<bool(VIPRA::idx)> const& cond) {
      return static_cast<module_t const*>(ptr)->conditional_closest_ped_impl(pedIdx, cond);
    };

    _allneighborswithin =
        +[](void const* ptr, VIPRA::idx pedIdx, VIPRA::f_pnt radius) -> std::vector<VIPRA::idx> {
      return static_cast<module_t const*>(ptr)->all_neighbors_within(pedIdx, radius);
    };

    // NOLINTEND(cppcoreguidelines-prefer-member-initializer)
  }

  // NOLINTBEGIN
  VIPRA_PEDS_INIT_STEP { throw std::runtime_error("Pedestrians View Should NOT Init"); }
  VIPRA_PEDS_UPDATE_STEP { throw std::runtime_error("Pedestrians View Should NOT Update"); }
  // NOLINTEND

  [[nodiscard]] auto distance(VIPRA::idx firstPed, VIPRA::idx secondPed) const -> VIPRA::f_pnt
  {
    return _dist(_module, firstPed, secondPed);
  }

  [[nodiscard]] auto num_pedestrians() const -> VIPRA::size { return _numpeds(_module); }

  [[nodiscard]] auto ped_coords(VIPRA::idx pedIdx) const -> VIPRA::f3d const&
  {
    return _pedcoords(_module, pedIdx);
  }

  [[nodiscard]] auto all_coords() const -> std::vector<VIPRA::f3d> const& { return _allcoords(_module); }

  [[nodiscard]] auto ped_velocity(VIPRA::idx pedIdx) const -> VIPRA::f3d const&
  {
    return _pedveloc(_module, pedIdx);
  }

  [[nodiscard]] auto all_velocities() const -> std::vector<VIPRA::f3d> const& { return _allveloc(_module); }

  template <typename condition_t>
  [[nodiscard]] auto closest_ped(VIPRA::idx pedIdx, condition_t&& condition = VOID{}) const -> VIPRA::idx
  {
    if constexpr ( std::is_same_v<condition_t, VOID> ) {
      return _closestped(_module, pedIdx);
    }
    else {
      return _condclosest(_module, pedIdx, std::forward<condition_t>(condition));
    }
  }

  [[nodiscard]] auto all_neighbors_within(VIPRA::idx   pedIdx,
                                          VIPRA::f_pnt radius) const -> std::vector<VIPRA::idx>
  {
    return _allneighborswithin(_module, pedIdx, radius);
  }
};
}  // namespace VIPRA::Views