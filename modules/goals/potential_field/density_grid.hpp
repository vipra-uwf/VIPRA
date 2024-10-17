#pragma once

#include <array>
#include <limits>
#include <queue>

#include "vipra/geometry/circle.hpp"
#include "vipra/geometry/f3d.hpp"

#include "vipra/types/float.hpp"
#include "vipra/types/idx.hpp"
#include "vipra/types/size.hpp"

#include "vipra/logging/logging.hpp"

// TODO: Generalize Grid and inherit Grid for this class.

namespace VIPRA::Goals {

class DensityGrid {
 public:
  struct GridPoint {
    VIPRA::f3d   direction;
    VIPRA::f3d   end{VIPRA::_emptyf3d_};
    VIPRA::f_pnt distance{std::numeric_limits<VIPRA::f_pnt>::max()};
    int          pedCount{0};
  };

  void intialize(auto const& map, VIPRA::f_pnt gridSize)
  {
    _gridSize = gridSize;
    construct_grid(map);
  }

  void clear_grid()
  {
    for ( GridPoint& gridPoint : _grid ) {
      gridPoint.pedCount = 0;
    }
  }

  [[nodiscard]] auto get_grid(VIPRA::f3d pos) -> GridPoint&
  {
    return _grid[get_closest_grid_idx(pos)];
  }
  [[nodiscard]] auto get_grid(VIPRA::f3d pos) const -> GridPoint const&
  {
    return _grid[get_closest_grid_idx(pos)];
  }

  [[nodiscard]] auto get_x_count() const -> size_t { return _xCount; }
  [[nodiscard]] auto get_y_count() const -> size_t { return _xCount; }

  [[nodiscard]] auto get_ped_count_at_idx(VIPRA::idx idx) const -> int
  {
    return _grid[idx].pedCount;
  }

  [[nodiscard]] auto begin() -> std::vector<GridPoint>::iterator
  {
    return _grid.begin();
  }
  [[nodiscard]] auto end() -> std::vector<GridPoint>::iterator
  {
    return _grid.end();
  }

  /**
   * @brief Increments the pedestrian count at the grid index that contains the point. 
   *
   * @param pos
   * @return VIPRA::idx
   */
  void incr_gridpoint(VIPRA::f3d const& pos)
  {
    const auto currentPedcount = _grid[get_closest_grid_idx(pos)].pedCount;
    _grid[get_closest_grid_idx(pos)].pedCount++;
  }

  /**
   * @brief Gets the closest grid index to the coordinate
   *
   * @param pos
   * @return VIPRA::idx
   */
  [[nodiscard]] auto get_closest_grid_idx(VIPRA::f3d pos) const -> VIPRA::idx
  {
    auto gridX = static_cast<VIPRA::idx>(std::floor(pos.x / _gridSize));
    auto gridY = static_cast<VIPRA::idx>(std::floor(pos.y / _gridSize));

    auto const idx = get_index(gridX, gridY, _xCount);

    if ( out_of_bounds(gridX, gridY) ) {
      VIPRA::Log::error("Grid index is out of bounds Pos: ({}, {})", pos.x,
                        pos.y);
      throw std::runtime_error("Grid index is out of bounds");
    }

    return idx;
  }

  [[nodiscard]] auto grid_center(VIPRA::f3d pos) const -> VIPRA::f3d
  {
    auto gridX = static_cast<VIPRA::idx>(std::floor(pos.x / _gridSize));
    auto gridY = static_cast<VIPRA::idx>(std::floor(pos.y / _gridSize));

    return VIPRA::f3d{_gridSize * static_cast<VIPRA::f_pnt>(gridX),
                      _gridSize * static_cast<VIPRA::f_pnt>(gridY)};
  }

 private:
  VIPRA::size  _xCount{};
  VIPRA::size  _yCount{};
  VIPRA::f_pnt _gridSize{};

  std::vector<GridPoint> _grid;

  /**
   * @brief Sets the number of grid points in the x and y directions
   *
   * @param map
   */
  void set_grid_counts(auto const& map)
  {
    const VIPRA::f3d dimensions = map.get_dimensions();

    assert(dimensions.x > 0 && dimensions.y > 0);
    assert(_gridSize > 0);

    _xCount = static_cast<VIPRA::idx>(std::ceil(dimensions.x / _gridSize) + 1);
    _yCount = static_cast<VIPRA::idx>(std::ceil(dimensions.y / _gridSize) + 1);
  }

  /**
   * @brief Gets the index of the grid point
   *
   * @param gridX
   * @param gridY
   * @param xCount
   * @return VIPRA::idx
   */
  [[nodiscard]] static auto get_index(VIPRA::size gridX, VIPRA::size gridY,
                                      VIPRA::size xCount) noexcept -> VIPRA::idx
  {
    return gridX + (gridY * xCount);
  }

  /**
     * @brief Constructs the graph of grid points
     *
     * @param map
     */
  void construct_grid(auto const& map)
  {
    set_grid_counts(map);

    assert(_xCount > 0 && _yCount > 0);

    _grid = std::vector<GridPoint>(_xCount * _yCount);
  }

  [[nodiscard]] VIPRA_INLINE auto out_of_bounds(
      VIPRA::f_pnt gridX, VIPRA::f_pnt gridY) const -> bool
  {
    return gridX < 0 ||
           gridX >= static_cast<VIPRA::f_pnt>(_xCount) * _gridSize ||
           gridY < 0 || gridY >= static_cast<VIPRA::f_pnt>(_yCount) * _gridSize;
  }

  [[nodiscard]] VIPRA_INLINE auto out_of_bounds(size_t gridX,
                                                size_t gridY) const -> bool
  {
    return gridX < 0 || gridX >= _xCount || gridY < 0 || gridY >= _yCount;
  }

 public:
  DensityGrid() = default;
  DensityGrid(const DensityGrid&) = default;
  DensityGrid(DensityGrid&&) noexcept = default;
  auto operator=(const DensityGrid&) -> DensityGrid& = default;
  auto operator=(DensityGrid&&) noexcept -> DensityGrid& = default;
  ~DensityGrid() = default;
};
}  // namespace VIPRA::Goals
