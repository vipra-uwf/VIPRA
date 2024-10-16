#pragma once

#include <array>
#include <limits>
#include <queue>

#include "vipra/geometry/circle.hpp"
#include "vipra/geometry/f3d.hpp"

#include "vipra/types/float.hpp"
#include "vipra/types/idx.hpp"
#include "vipra/types/size.hpp"

#include "density_grid.hpp"
#include "vipra/logging/logging.hpp"

// TODO: Generalize Grid and inherit Grid for this class.

namespace VIPRA::Goals {

class Grid {
 public:
  struct GridPoint {
    VIPRA::f3d   direction;
    VIPRA::f3d   end{VIPRA::_emptyf3d_};
    VIPRA::f_pnt distance{std::numeric_limits<VIPRA::f_pnt>::max()};
  };

  void intialize(auto const& map, VIPRA::f_pnt gridSize)
  {
    _gridSize = gridSize;
    construct_grid(map);
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

  [[nodiscard]] auto begin() -> std::vector<GridPoint>::iterator
  {
    return _grid.begin();
  }
  [[nodiscard]] auto end() -> std::vector<GridPoint>::iterator
  {
    return _grid.end();
  }

  void flood_fill(VIPRA::f3d start, auto const& map)
  {
    const VIPRA::f3d       dimensions = map.get_dimensions();
    std::queue<VIPRA::f3d> next;
    VIPRA::f_pnt           dist = 0;

    get_grid(start).distance = 0;
    next.push(grid_center(start));

    while ( ! next.empty() ) {
      VIPRA::f3d currPos = next.front();
      next.pop();

      if ( map.collision(Geometry::Circle{grid_center(currPos), _gridSize}) )
        continue;

      add_neighbors(currPos, start, next, 0);
    }
  }

  void flood_fill(VIPRA::f3d start, auto const& map,
                  DensityGrid const& densityGrid)
  {
    const VIPRA::f3d       dimensions = map.get_dimensions();
    std::queue<VIPRA::f3d> next;
    VIPRA::f_pnt           dist = 0;

    get_grid(start).distance = 0;
    next.push(grid_center(start));

    while ( ! next.empty() ) {
      VIPRA::f3d currPos = next.front();
      next.pop();

      if ( map.collision(Geometry::Circle{grid_center(currPos), _gridSize}) )
        continue;

      VIPRA::idx densityGridIndex = densityGrid.get_closest_grid_idx(currPos);
      int        pedCount = densityGrid.get_ped_count_at_idx(densityGridIndex);
      VIPRA::Log::debug("Adding Weight: {}", pedCount);
      add_neighbors(currPos, start, next, pedCount);
    }
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

  void add_neighbors(VIPRA::f3d curr, VIPRA::f3d end,
                     std::queue<VIPRA::f3d>& queue, VIPRA::f_pnt weight)
  {
    std::array<std::pair<int, int>, 8> const directions{
        {{-1, 1}, {0, 1}, {1, 1}, {-1, 0}, {1, 0}, {-1, -1}, {0, -1}, {1, -1}}};

    auto& currGrid = get_grid(curr);

    for ( auto [i, j] : directions ) {
      if ( i == 0 && i == j ) continue;

      VIPRA::f3d nextPos =
          VIPRA::f3d{curr.x + i * _gridSize, curr.y + j * _gridSize};

      if ( out_of_bounds(nextPos.x, nextPos.y) ) continue;

      auto& grid = get_grid(nextPos);

      // VIPRA::Log::debug("distance: {} | distance_to_sqrd: {} | additional weight: {}", currGrid.distance, curr.distance_to_sqrd(nextPos), (weight * 0.01));
      VIPRA::f_pnt dist =
          currGrid.distance + curr.distance_to_sqrd(nextPos) + weight;

      if ( grid.distance <= dist ) continue;

      grid.direction = (curr - nextPos).unit();
      grid.end = end;
      grid.distance = dist;

      queue.emplace(nextPos);
    }
  }

 public:
  Grid() = default;
  Grid(const Grid&) = default;
  Grid(Grid&&) noexcept = default;
  auto operator=(const Grid&) -> Grid& = default;
  auto operator=(Grid&&) noexcept -> Grid& = default;
  ~Grid() = default;
};
}  // namespace VIPRA::Goals
