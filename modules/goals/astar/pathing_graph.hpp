#pragma once

#include <array>
#include <limits>

#include "vipra/geometry/f3d.hpp"
#include "vipra/macros/performance.hpp"
#include "vipra/modules/map.hpp"
#include "vipra/types/float.hpp"
#include "vipra/types/idx.hpp"
#include "vipra/types/size.hpp"

namespace VIPRA::Goals {
struct GridPoint {
  std::array<bool, 8> neighbors;
  bool                traversable;
};

class PathingGraph {
 public:
  [[nodiscard]] VIPRA_INLINE auto node_count() const noexcept -> size_t
  {
    return _grids.size();
  }

  [[nodiscard]] auto neighbors(VIPRA::idx gridIdx) const noexcept
      -> std::array<VIPRA::idx, 8>
  {
    assert(gridIdx < _grids.size());

    std::array<VIPRA::idx, 8> neighbors{};

    VIPRA::idx nIdx = 0;
    for ( bool neighbor : _grids[gridIdx].neighbors ) {
      if ( ! neighbor || (neighbor_idx(gridIdx, nIdx) > _grids.size() ||
                          neighbor_idx(gridIdx, nIdx) < 0) ) {
        neighbors[nIdx] = std::numeric_limits<VIPRA::idx>::max();
        ++nIdx;
        continue;
      }

      neighbors[nIdx] = neighbor_idx(gridIdx, nIdx);
      ++nIdx;
    }

    return neighbors;
  }

  [[nodiscard]] VIPRA_INLINE auto pos(VIPRA::idx gridIdx) const noexcept
      -> VIPRA::f3d
  {
    assert(gridIdx < _grids.size());
    return _positions[gridIdx];
  }
  /**
   * @brief Gets the closest grid index to the coordinate
   *
   * @param pos
   * @return VIPRA::idx
   */
  [[nodiscard]] auto get_closest_grid_idx(VIPRA::f3d position) const noexcept
      -> VIPRA::idx
  {
    assert(position <= pos(get_index(gridX, gridY, _xCount)));

    auto const gridX =
        static_cast<VIPRA::idx>(std::floor(position.x / _gridSize));
    auto const gridY =
        static_cast<VIPRA::idx>(std::floor(position.y / _gridSize));

    return get_index(gridX, gridY);
  }

 private:
  VIPRA::size  _xCount{};
  VIPRA::size  _yCount{};
  VIPRA::f_pnt _gridSize{};
  VIPRA::f_pnt _closestObstacle{};

  std::vector<GridPoint>  _grids;
  std::vector<VIPRA::f3d> _positions;

  void set_grid_counts(VIPRA::Modules::Map const& map);
  void construct_graph(VIPRA::Modules::Map const& map);
  void set_adjacents(VIPRA::idx currIdx);

  [[nodiscard]] auto neighbor_idx(
      VIPRA::idx gridIdx, VIPRA::idx neighbor) const noexcept -> VIPRA::idx
  {
    const int                xCount = static_cast<int>(_xCount);
    const std::array<int, 8> deltaIdx = {
        (-xCount - 1), -xCount, (-xCount + 1), -1, 1,
        (xCount - 1),  xCount,  (xCount + 1),
    };

    return gridIdx + deltaIdx[neighbor];
  }

  /**
   * @brief Gets the index of the grid point
   *
   * @param gridX
   * @param gridY
   * @param xCount
   * @return VIPRA::idx
   */
  [[nodiscard]] VIPRA_INLINE auto get_index(
      VIPRA::size gridX, VIPRA::size gridY) const noexcept -> VIPRA::idx
  {
    return gridX + (gridY * _xCount);
  }

 public:
  PathingGraph(Modules::Map const& map, VIPRA::f_pnt gridSize,
               VIPRA::f_pnt closestObstacle)
      : _gridSize(gridSize), _closestObstacle(closestObstacle)
  {
    construct_graph(map);
  }
  PathingGraph() = default;
  PathingGraph(const PathingGraph&) = default;
  PathingGraph(PathingGraph&&) = default;
  auto operator=(const PathingGraph&) -> PathingGraph& = default;
  auto operator=(PathingGraph&&) -> PathingGraph& = default;
  ~PathingGraph() = default;
};
}  // namespace VIPRA::Goals
