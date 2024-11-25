#pragma once

// TODO(rolland): issue #16 complete proper implementation

#include <stdexcept>
#include <vector>

#include "vipra/geometry/f3d.hpp"

#include "vipra/macros/performance.hpp"

#include "vipra/types/float.hpp"
#include "vipra/types/idx.hpp"
#include "vipra/types/size.hpp"

namespace VIPRA::DataStructures {
template <typename data_t>
class SpatialMap {
 public:
  void update_grids(std::vector<VIPRA::f3d> const& oldPositions,
                    std::vector<VIPRA::f3d> const& newPositions)
  {
    // for all of the data, check if they moved grids, if they did update both
    for ( VIPRA::idx currIdx = 0; currIdx < newPositions.size(); ++currIdx ) {
      auto& oldGrid = get_grid(oldPositions[currIdx]);
      auto& newGrid = get_grid(newPositions[currIdx]);

      if ( &oldGrid == &newGrid ) continue;

      oldGrid.erase(std::remove(oldGrid.begin(), oldGrid.end(), currIdx),
                    oldGrid.end());
      newGrid.push_back(currIdx);
    }
  }

  void for_each_neighbor(VIPRA::f3d pos, auto&& func) const
  {
    // TODO(rolland, issue #40) this only gets the surrounding 9 grids

    const VIPRA::idx gridX = pos.x / _cellSize;
    const VIPRA::idx gridY = pos.y / _cellSize;

    // Loop through the surrounding grids and call the provided function with each pedestrian in each grid
    for ( int i = -1; i <= 1; ++i ) {
      for ( int j = -1; j <= 1; ++j ) {
        if ( out_of_bounds(gridX + i, gridY + j) ) continue;

        auto const& neighbor = _grid[std::floor(pos.x / _cellSize) +
                                     std::floor(pos.y / _cellSize) * _cols];

        for ( auto value : neighbor ) {
          func(value);
        }
      }
    }
  }

  void initialize(VIPRA::f_pnt cellSize, VIPRA::f_pnt width,
                  VIPRA::f_pnt height, std::vector<VIPRA::f3d> const& positions,
                  std::vector<data_t> const& data)
  {
    assert(cellSize != 0);
    assert(width != 0);
    assert(height != 0);

    _cellSize = cellSize;
    set_grids(width, height);
    initialize_grids(positions, data);
  }

 private:
  std::vector<std::vector<data_t>> _grid;
  VIPRA::size                      _rows{};
  VIPRA::size                      _cols{};
  VIPRA::f_pnt                     _cellSize{};

  void set_grids(VIPRA::f_pnt width, VIPRA::f_pnt height)
  {
    _rows = static_cast<size_t>(std::ceil(height / _cellSize));
    _cols = static_cast<size_t>(std::ceil(width / _cellSize));
    _grid.resize(_rows * _cols);
  }

  void initialize_grids(std::vector<VIPRA::f3d> const& positions,
                        std::vector<data_t> const&     data)
  {
    if ( positions.size() != data.size() )
      throw std::logic_error(
          "Spatial Map initialized with differing counts of data and "
          "positions");

    // Create grids
    for ( auto& grid : _grid ) {
      grid = std::vector<data_t>();
    }

    // Add all of the pedestrians in each grid to their respective grid
    for ( VIPRA::idx currIdx = 0; currIdx < positions.size(); ++currIdx ) {
      auto& grid = get_grid(positions[currIdx]);
      grid.push_back(data[currIdx]);
    }
  }

  /**
   * @brief Returns the grid for a given position
   *
   * @param pos
   * @return std::vector<VIPRA::idx>&
   */
  [[nodiscard]] VIPRA_INLINE auto get_grid(VIPRA::f3d pos)
      -> std::vector<VIPRA::idx>&
  {
    if ( out_of_bounds(std::floor(pos.x / _cellSize),
                       std::floor(pos.y / _cellSize)) ) {
      std::string posStr =
          "{" + std::to_string(pos.x) + ", " + std::to_string(pos.y) + "}\n";
      throw std::runtime_error(posStr.c_str());
    }

    assert(! out_of_bounds(pos));

    return _grid[std::floor(pos.x / _cellSize) +
                 std::floor(pos.y / _cellSize) * _cols];
  }

  /**
   * @brief Returns the grid for a given position
   * 
   * @param pos 
   * @return std::vector<VIPRA::idx> const& 
   */
  [[nodiscard]] VIPRA_INLINE auto get_grid(VIPRA::f3d pos) const
      -> std::vector<VIPRA::idx> const&
  {
    return get_grid(pos);
  }

  /**
   * @brief Checks if the point is out of bounds
   * 
   * @param x 
   * @param y 
   * @return true 
   * @return false 
   */
  [[nodiscard]] VIPRA_INLINE auto out_of_bounds(VIPRA::idx gridX,
                                                VIPRA::idx gridY) const -> bool
  {
    return gridX < 0 || gridX >= _cols || gridY < 0 || gridY >= _rows;
  }

  /**
   * @brief Checks if the point is out of bounds
   * 
   * @param x 
   * @param y 
   * @return true 
   * @return false 
   */
  [[nodiscard]] VIPRA_INLINE auto out_of_bounds(VIPRA::f3d pos) const -> bool
  {
    return out_of_bounds(std::floor(pos.x / _cellSize),
                         std::floor(pos.y / _cellSize));
  }
};
}  // namespace VIPRA::DataStructures