#pragma once

#include "vipra/data_structures/graph.hpp"

#include "vipra/geometry/circle.hpp"
#include "vipra/types/float.hpp"
#include "vipra/types/idx.hpp"
#include "vipra/types/size.hpp"

namespace VIPRA::Goals {
struct GridPoint {
  VIPRA::f3d pos;
  bool       traversable;
};

class PathingGraph : public VIPRA::DataStructures::GraphMixin<PathingGraph, GridPoint> {
 public:
  template <typename obstacles_t>
  PathingGraph(obstacles_t const& obstacles, VIPRA::f_pnt gridSize, VIPRA::f_pnt closestObstacle)
      : _gridSize(gridSize), _closestObstacle(closestObstacle)
  {
    construct_graph(obstacles);
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

    if ( idx >= nodes().size() ) {
      throw std::runtime_error("Grid index is out of bounds");
    }

    return idx;
  }

 private:
  VIPRA::size  _xCount{};
  VIPRA::size  _yCount{};
  VIPRA::f_pnt _gridSize{};
  VIPRA::f_pnt _closestObstacle{};

  /**
   * @brief Sets the number of grid points in the x and y directions
   *
   * @param obstacles
   */
  void set_grid_counts(auto const& obstacles)
  {
    const VIPRA::f3d dimensions = obstacles.get_dimensions();

    assert(dimensions.x > 0 && dimensions.y > 0);
    assert(_gridSize > 0 && _closestObstacle > 0);

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
  [[nodiscard]] static auto get_index(VIPRA::size gridX, VIPRA::size gridY, VIPRA::size xCount) noexcept
      -> VIPRA::idx
  {
    return gridX + (gridY * xCount);
  }

  /**
     * @brief Constructs the graph of grid points
     *
     * @param obstacles
     */
  void construct_graph(auto const& obstacles)
  {
    clear();
    set_grid_counts(obstacles);

    assert(_xCount > 0 && _yCount > 0);

    reserve(_xCount * _yCount);

    // TODO(rolland): issue #14 This starts the grid a (0, 0) and not the bottom left corner of the map, we may want to change this
    VIPRA::f3d center;

    // Construct a graph of grids filling the map with adjacent grids connecting
    for ( VIPRA::idx currY = 0; currY < _yCount; ++currY ) {
      center.x = 0.0F;
      center.y += _gridSize;

      for ( VIPRA::idx currX = 0; currX < _xCount; ++currX ) {
        center.x += _gridSize;
        const VIPRA::idx currIdx = add_node(
            GridPoint{center, ! obstacles.collision(VIPRA::Geometry::Circle{center, _closestObstacle})});

        if ( ! data(currIdx).traversable ) {
          continue;
        }

        set_adjacents(currIdx, currX, currY);
      }
    }
  }

  /**
     * @brief Set the adjacent nodes of the current node
     *
     * @param currIdx
     * @param currX
     * @param currY
     */
  // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
  void set_adjacents(VIPRA::idx currIdx, VIPRA::idx currX, VIPRA::idx currY)
  {
    assert(currIdx < nodes().size());
    assert(currX < _xCount && currY < _yCount);
    assert(data(currIdx).traversable == true);

    // Check if the neighbor grid is eligible to be connected to
    if ( currX > 1 ) {
      if ( data(currIdx - 1).traversable ) add_edge(currIdx, currIdx - 1);
    }
    if ( currY > 1 ) {
      if ( data(currIdx - _xCount).traversable ) add_edge(currIdx, currIdx - _xCount);
    }
    if ( currX > 1 && currY > 1 ) {
      if ( data(currIdx - _xCount - 1).traversable ) add_edge(currIdx, currIdx - _xCount - 1);
    }
    if ( currX < _xCount - 1 && currY > 1 ) {
      if ( data(currIdx - _xCount + 1).traversable ) add_edge(currIdx, currIdx - _xCount + 1);
    }
  }

 public:
  PathingGraph() = default;
  PathingGraph(const PathingGraph&) = default;
  PathingGraph(PathingGraph&&) = default;
  auto operator=(const PathingGraph&) -> PathingGraph& = default;
  auto operator=(PathingGraph&&) -> PathingGraph& = default;
  ~PathingGraph() = default;
};
}  // namespace VIPRA::Goals
