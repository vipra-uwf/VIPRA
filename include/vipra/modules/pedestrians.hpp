#pragma once

#include <type_traits>
#include <vector>

#include "vipra/logging/logging.hpp"
#include "vipra/macros/parameters.hpp"

#include "vipra/geometry/f3d.hpp"

#include "vipra/modules/util.hpp"
#include "vipra/random/distributions.hpp"
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
 * @brief Base Pedestrians module
 * 
 */
template <typename module_t>
class Pedestrians : public Util::CRTP<Pedestrians<module_t>> {
  using Util::CRTP<Pedestrians<module_t>>::self;

 public:
  REGISTER_BASE_PARAMS(VIPRA_PARAM("random_count", _randomPedCnt), VIPRA_PARAM("spawn_random", _randomSpawn))

  void initialize(auto& input, auto const& map, VIPRA::Random::Engine& engine)
  {
    init_specific_peds(input);

    if ( _randomSpawn && _randomPedCnt > 0 ) {
      init_random_peds(map, engine);
    }

    assert(_coords.size() > 0);
    assert(_coords.size() == _velocities.size());

    VIPRA::Log::debug("{} total pedestrians added", _coords.size());

    self().init_step(map, engine);
  }

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

  size_t _randomPedCnt{0};
  bool   _randomSpawn{false};

 protected:
  void set_velocities(VIPRA::f3dVec const& velocities) { _velocities = velocities; }
  void set_velocities(VIPRA::f3dVec&& velocities) noexcept { _velocities = velocities; }

  void set_coordinates(VIPRA::f3dVec const& coordinates) { _coords = coordinates; }
  void set_coordinates(VIPRA::f3dVec&& coordinates) noexcept { _coords = coordinates; }

  [[nodiscard]] auto get_velocities() const -> VIPRA::f3dVec const& { return _velocities; }
  [[nodiscard]] auto get_coordinates() const -> VIPRA::f3dVec const& { return _coords; }

  void init_random_peds(auto const& map, VIPRA::Random::Engine& engine)
  {
    // TODO(rolland, issue #50): this is terrible

    assert(_coords.size() == _velocities.size());

    auto spawnAreas = map.template get_spawns();
    if ( spawnAreas.size() == 0 ) {
      VIPRA::Log::warn(
          "{} Pedestrians were set to spawn randomly, but no spawn areas were provided in the map",
          _randomPedCnt);
      return;
    }

    size_t startSize = _coords.size();
    _coords.resize(_coords.size() + _randomPedCnt);
    _velocities.resize(_velocities.size() + _randomPedCnt);

    VIPRA::Random::uniform_distribution<size_t> polyDist{0, spawnAreas.size() - 1};

    // go through each pedestrian and give them a random point inside one of the spawn points
    for ( size_t i = startSize; i < _coords.size(); ++i ) {
      // TODO(rolland, issue #50): effectively infinite loop if the spawn is covered by an obstacle
      do {
        _coords[i] = spawnAreas[polyDist(engine)].random_point(engine);
      } while ( ! map.collision(_coords[i]) );
    }

    VIPRA::Log::debug("{} pedestrians added randomly to {} spawn areas", _randomPedCnt, spawnAreas.size());
  }

  void init_specific_peds(auto& input)
  {
    if constexpr ( std::is_same_v<VOID, decltype(input)> ) {
      // no input provided, just return
      return;
    }
    else {
      auto coords = input.template get<std::vector<VIPRA::f3d>>({"pedestrians"});
      if ( ! (coords || _randomSpawn) )
        throw std::runtime_error(
            "Could not find pedestrian coordinates in input file and random spawning is turned off (No "
            "Pedestrians Added)");

      VIPRA::Log::debug("{} pedestrians added at {} specific spots", (*coords).size(), (*coords).size());
      set_coordinates(std::move(*coords));
      set_velocities(std::vector<VIPRA::f3d>(_coords.size()));
    }
  }
};
}  // namespace VIPRA::Modules