#pragma once

#include <iostream>

#include "vipra/concepts/input.hpp"
#include "vipra/concepts/pedset.hpp"
#include "vipra/macros/module.hpp"
#include "vipra/macros/parameters.hpp"
#include "vipra/macros/pedestrians.hpp"
#include "vipra/macros/performance.hpp"
#include "vipra/modules.hpp"

#include "vipra/geometry/f3d.hpp"
#include "vipra/types/float.hpp"
#include "vipra/types/idx.hpp"
#include "vipra/types/size.hpp"
#include "vipra/types/state.hpp"

#include "vipra/types/util/result_or_void.hpp"
#include "vipra/util/debug_do.hpp"

// TODO(rolland): implement grid for storing pedestrians

namespace VIPRA::Pedestrians {

/**
 * @brief Pedestrian module that uses a grid to store pedestrians
 * 
 */
template <Concepts::InputModule input_t>
class Grid {
 public:
  VIPRA_MODULE_TYPE(PEDESTRIANS);
  VIPRA_MODULE_NAME("grid")

  // TODO(rolland): having the pedset call load means each node will load the same file
  explicit Grid(input_t&& input) : _input(input) { _input.load(); }

  VIPRA_PEDS_INIT_STEP {
    auto coords = _input.template get<std::vector<VIPRA::f3d>>("coords");
    if ( ! coords ) throw std::runtime_error("Could not find pedestrian coordinates in input file");

    auto dimensions = map.get_dimensions();
    set_grids(dimensions.x, dimensions.y);

    _velocities = std::vector<VIPRA::f3d>((*coords).size());
    _coords = std::move(*coords);

    initialize_grids();
  }

  VIPRA_PEDS_UPDATE_STEP {
    assert(state.size() == _coords.size());

    // Update pedestrian positions in grids
    update_grids(state);
    for ( VIPRA::idx pedIdx = 0; pedIdx < state.size(); ++pedIdx ) {
      _coords[pedIdx] = state.positions[pedIdx];
      _velocities[pedIdx] = state.velocities[pedIdx];
    }
  }

  VIPRA_REGISTER_STEP { VIPRA_REGISTER_PARAM("gridSize"); }

  VIPRA_CONFIG_STEP { VIPRA_GET_PARAM("gridSize", _cellSize); }

  [[nodiscard]] auto closest_ped(VIPRA::idx ped, auto&& condition = VOID{}) const -> VIPRA::idx {
    VIPRA_PERF_FUNCTION("grid::closest_ped")

    VIPRA::f_pnt minDist = std::numeric_limits<VIPRA::f_pnt>::max();
    VIPRA::idx   minIdx = ped;

    // Check all surrounding grids for the nearest pedestrian that matches the predicate
    // TODO(rolland): this only checks the 9 neighbors, should check all until a certain distance
    for_each_neighbor(_coords[ped], [&](VIPRA::idx other) {
      VIPRA::f_pnt dist = _coords[ped].distance_to(_coords[other]);

      if ( dist < minDist ) {
        if constexpr ( ! std::is_same_v<decltype(condition), VOID> ) {
          // don't have a predicate, return the nearest
          if ( ! condition(other) ) return;
        }

        minDist = dist;
        minIdx = other;
      }
    });

    return minIdx;
  }

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

  std::vector<std::vector<VIPRA::idx>> _grid;
  size_t                               _rows{};
  size_t                               _cols{};
  VIPRA::f_pnt                         _cellSize{};

  void set_grids(VIPRA::f_pnt width, VIPRA::f_pnt height) {
    _rows = static_cast<size_t>(std::ceil(height / _cellSize));
    _cols = static_cast<size_t>(std::ceil(width / _cellSize));
    _grid.resize(_rows * _cols);
  }

  void initialize_grids() {
    // Create grids
    for ( auto& grid : _grid ) {
      grid = std::vector<VIPRA::idx>();
    }

    // Add all of the pedestrians in each grid to their respective grid
    for ( VIPRA::idx pedIdx = 0; pedIdx < _coords.size(); ++pedIdx ) {
      auto& grid = get_grid(_coords[pedIdx]);
      grid.push_back(pedIdx);
    }
  }

  void update_grids(VIPRA::State const& state) {
    VIPRA_PERF_FUNCTION("grid::update_grids")

    // for all of the pedestrians, check if they moved grids, if they did update both
    for ( VIPRA::idx pedIdx = 0; pedIdx < state.size(); ++pedIdx ) {
      auto& oldGrid = get_grid(_coords[pedIdx]);
      auto& newGrid = get_grid(state.positions[pedIdx]);

      if ( &oldGrid == &newGrid ) continue;

      oldGrid.erase(std::remove(oldGrid.begin(), oldGrid.end(), pedIdx), oldGrid.end());
      newGrid.push_back(pedIdx);
    }
  }

  /**
   * @brief Returns the grid for a given position
   * 
   * @param pos 
   * @return std::vector<VIPRA::idx>& 
   */
  inline auto get_grid(VIPRA::f3d pos) -> std::vector<VIPRA::idx>& {
    assert(! out_of_bounds(pos));

    auto x = static_cast<size_t>(pos.x / _cellSize);
    auto y = static_cast<size_t>(pos.y / _cellSize);

    return _grid[x + y * _cols];
  }

  [[nodiscard]] inline auto get_grid(VIPRA::f3d pos) const -> std::vector<VIPRA::idx> const& {
    return _grid[static_cast<size_t>(pos.x / _cellSize) + static_cast<size_t>(pos.y / _cellSize) * _cols];
  }

  [[nodiscard]] inline auto out_of_bounds(VIPRA::f_pnt x, VIPRA::f_pnt y) const -> bool {
    return x < 0 || x >= _cols * _cellSize || y < 0 || y >= _rows * _cellSize;
  }

  inline auto for_each_neighbor(VIPRA::f3d pos, auto&& func) const {
    VIPRA_PERF_FUNCTION("grid::for_each_neighbor")

    // Loop through the surrounding grids and call the provided function with each pedestrian in each grid
    for ( int i = -1; i <= 1; ++i ) {
      for ( int j = -1; j <= 1; ++j ) {
        if ( out_of_bounds(pos.x + i * _cellSize, pos.y + j * _cellSize) ) continue;

        auto& neighbor = get_grid(VIPRA::f3d{pos.x + i * _cellSize, pos.y + j * _cellSize});

        for ( auto pedIdx : neighbor ) {
          func(pedIdx);
        }
      }
    }
  }
};
}  // namespace VIPRA::Pedestrians

CHECK_MODULE(PedsetModule, VIPRA::Pedestrians::Grid<VIPRA::Concepts::DummyInput>)