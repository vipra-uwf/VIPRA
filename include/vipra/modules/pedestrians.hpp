#pragma once

#include <functional>
#include <vector>

#include "vipra/geometry/circle.hpp"
#include "vipra/logging/logging.hpp"

#include "vipra/macros/errors.hpp"
#include "vipra/macros/module.hpp"
#include "vipra/modules/map.hpp"

#include "vipra/geometry/f3d.hpp"

#include "vipra/modules/module.hpp"
#include "vipra/modules/pedestrian_input.hpp"

#include "vipra/random/distributions.hpp"
#include "vipra/random/random.hpp"

#include "vipra/types/float.hpp"
#include "vipra/types/idx.hpp"
#include "vipra/types/size.hpp"
#include "vipra/types/state.hpp"

#include "vipra/macros/parameters.hpp"
#include "vipra/macros/pedestrians.hpp"

namespace VIPRA::Modules {

/**
 * @brief Base Pedestrians module
 * 
 */
class Pedestrians : public BaseModule<Pedestrians> {
 public:
  virtual VIPRA_PEDS_INIT_STEP = 0;
  virtual VIPRA_PEDS_UPDATE_STEP = 0;
  [[nodiscard]] virtual auto all_neighbors_within(VIPRA::idx   pedIdx,
                                                  VIPRA::f_pnt radius) const
      -> std::vector<VIPRA::idx> = 0;

  [[nodiscard]] virtual auto closest_ped(VIPRA::idx pedIdx) const
      -> VIPRA::idx = 0;

  [[nodiscard]] virtual auto conditional_closest_ped(
      VIPRA::idx                             pedIdx,
      std::function<bool(VIPRA::idx)> const& condition) const -> VIPRA::idx = 0;

  VIPRA_MODULE_TYPE(Pedestrians);

  VIPRA_REGISTER_BASE_PARAMS(VIPRA_PARAM("random_count", _randomPedCnt),
                             VIPRA_PARAM("spawn_random", _randomSpawn),
                             VIPRA_PARAM("use_file", _useFile))

  void initialize(VIPRA::Modules::PedestrianInput& input,
                  VIPRA::Modules::Map const& map, VIPRA::Random::Engine& engine)
  {
    VIPRA::Log::debug("Initializing Pedestrians");
    if ( ! (_randomSpawn || _useFile) )
      VIPRA_BASE_MODULE_ERROR(
          "Pedestrians",
          "Random spawning AND Using the pedestrian file input is turned off "
          "in parameters (Cannot Add any "
          "Pedestrians, "
          "one needs to be true)");

    init_specific_peds(input);

    if ( _randomSpawn && _randomPedCnt > 0 ) {
      init_random_peds(map, engine);
    }

    if ( _coords.size() == 0 )
      VIPRA_BASE_MODULE_ERROR("Pedestrians",
                              "No Pedestrians added to simulation");

    assert(_coords.size() > 0);
    assert(_coords.size() == _velocities.size());

    init_step(map, engine);
    VIPRA::Log::debug("Pedestrians Initialized");
  }

  void update(VIPRA::State const& state)
  {
    update_step(state);

    for ( VIPRA::idx pedIdx = 0; pedIdx < state.size(); ++pedIdx ) {
      _coords[pedIdx] = state.positions[pedIdx];
      _velocities[pedIdx] = state.velocities[pedIdx];
    }
  }

  [[nodiscard]] auto distance(VIPRA::idx firstPed,
                              VIPRA::idx secondPed) const -> VIPRA::f_pnt
  {
    return _coords[firstPed].distance_to(_coords[secondPed]);
  }

  [[nodiscard]] auto num_pedestrians() const -> VIPRA::size
  {
    return _coords.size();
  }

  [[nodiscard]] auto ped_coords(VIPRA::idx pedIdx) const -> VIPRA::f3d const&
  {
    assert(pedIdx < _coords.size());

    return _coords[pedIdx];
  }

  [[nodiscard]] auto all_coords() const -> std::vector<VIPRA::f3d> const&
  {
    return _coords;
  }

  [[nodiscard]] auto ped_velocity(VIPRA::idx pedIdx) const -> VIPRA::f3d const&
  {
    assert(pedIdx < _velocities.size());
    return _velocities[pedIdx];
  }

  [[nodiscard]] auto all_velocities() const -> std::vector<VIPRA::f3d> const&
  {
    return _velocities;
  }

 private:
  VIPRA::f3dVec _coords;
  VIPRA::f3dVec _velocities;

  size_t _randomPedCnt{0};
  bool   _randomSpawn{false};
  bool   _useFile{true};

 protected:
  void set_velocities(VIPRA::f3dVec const& velocities)
  {
    _velocities = velocities;
  }
  void set_velocities(VIPRA::f3dVec&& velocities) noexcept
  {
    _velocities = velocities;
  }

  void set_coordinates(VIPRA::f3dVec const& coordinates)
  {
    _coords = coordinates;
  }
  void set_coordinates(VIPRA::f3dVec&& coordinates) noexcept
  {
    _coords = coordinates;
  }

  [[nodiscard]] auto get_velocities() const -> VIPRA::f3dVec const&
  {
    return _velocities;
  }
  [[nodiscard]] auto get_coordinates() const -> VIPRA::f3dVec const&
  {
    return _coords;
  }

  void init_random_peds(VIPRA::Modules::Map const& map,
                        VIPRA::Random::Engine&     engine)
  {
    // TODO(rolland, issue #50): this is terrible

    assert(_coords.size() == _velocities.size());

    auto spawnAreas = map.get_spawns();
    if ( spawnAreas.size() == 0 ) {
      VIPRA::Log::warn(
          "{} Pedestrians were set to spawn randomly, but no spawn areas were "
          "provided in the map",
          _randomPedCnt);
      return;
    }

    size_t startSize = _coords.size();
    _coords.resize(_coords.size() + _randomPedCnt);
    _velocities.resize(_velocities.size() + _randomPedCnt);

    VIPRA::Random::uniform_distribution<size_t> polyDist{0,
                                                         spawnAreas.size() - 1};

    // go through each pedestrian and give them a random point inside one of the spawn points
    for ( size_t i = startSize; i < _coords.size(); ++i ) {
      // TODO(rolland, issue #50): effectively infinite loop if the spawn is covered by an obstacle
      VIPRA::idx spawnIdx = polyDist(engine);

      // if area 0

      do {
        _coords[i] = spawnAreas[spawnIdx].random_point(engine);
        VIPRA::Log::debug("Random Point: ({}, {}) Collides: {}", _coords[i].x,
                          _coords[i].y, map.collision(_coords[i]));
      } while ( ! map.collision(Geometry::Circle{_coords[i], 0.4}) );
    }

    VIPRA::Log::debug("{} pedestrians added randomly to {} spawn areas",
                      _randomPedCnt, spawnAreas.size());
  }

  void init_specific_peds(VIPRA::Modules::PedestrianInput& input)
  {
    if ( ! _useFile ) return;

    auto coords = input.get_pedestrians();
    if ( ! (coords || _randomSpawn) )
      VIPRA_BASE_MODULE_ERROR(
          "Pedestrians",
          "Could not find pedestrian coordinates in input file and random "
          "spawning is turned off (No "
          "Pedestrians Added)");

    if ( ! coords )
      VIPRA::Log::warn(
          "Pedestrians were set to be loaded from a file, but no coordinates "
          "could be loaded");

    VIPRA::Log::debug("{} pedestrians added at {} specific spots",
                      (*coords).size(), (*coords).size());
    set_coordinates(std::move(*coords));
    set_velocities(std::vector<VIPRA::f3d>(_coords.size()));
  }

 public:
  virtual ~Pedestrians() = default;
  Pedestrians() = default;
  Pedestrians(Pedestrians const&) = default;
  Pedestrians(Pedestrians&&) noexcept = default;
  auto operator=(Pedestrians const&) -> Pedestrians& = default;
  auto operator=(Pedestrians&&) noexcept -> Pedestrians& = default;
};
}  // namespace VIPRA::Modules