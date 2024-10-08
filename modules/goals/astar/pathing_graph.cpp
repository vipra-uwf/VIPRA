
#include "pathing_graph.hpp"

namespace VIPRA::Goals {

void PathingGraph::set_grid_counts(VIPRA::Modules::Map const& map)
{
  const VIPRA::f3d dimensions = map.get_dimensions();

  assert(dimensions.x > 0 && dimensions.y > 0);
  assert(_gridSize > 0 && _closestObstacle > 0);

  _xCount = static_cast<VIPRA::idx>(std::ceil(dimensions.x / _gridSize) + 1);
  _yCount = static_cast<VIPRA::idx>(std::ceil(dimensions.y / _gridSize) + 1);
  _grids.resize(_xCount * _yCount);
  _positions.resize(_xCount * _yCount);
}

/**
  * @brief Constructs the graph of grid points
  *
  * @param map
  */
void PathingGraph::construct_graph(VIPRA::Modules::Map const& map)
{
  set_grid_counts(map);

  assert(_xCount > 0 && _yCount > 0);

  // TODO(rolland): issue #14 This starts the grid a (0, 0) and not the bottom left corner of the map, we may want to change this

  VIPRA::f3d center;
  // Construct a graph of grids filling the map with adjacent grids connecting
  for ( VIPRA::idx currY = 0; currY < _yCount; ++currY ) {
    center.x = _gridSize;
    center.y += _gridSize;

    for ( VIPRA::idx currX = 0; currX < _xCount; ++currX ) {
      center.x += _gridSize;
      _positions[get_index(currX, currY)] = center;
      _grids[get_index(currX, currY)].traversable =
          ! map.collision(VIPRA::Geometry::Circle{center, _closestObstacle});
    }
  }

  // Construct a graph of grids filling the map with adjacent grids connecting
  for ( VIPRA::idx currY = 0; currY < _yCount; ++currY ) {
    for ( VIPRA::idx currX = 0; currX < _xCount; ++currX ) {
      const VIPRA::idx currIdx = get_index(currX, currY);
      if ( _grids[currIdx].traversable ) set_adjacents(currIdx);
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
void PathingGraph::set_adjacents(VIPRA::idx currIdx)
{
  assert(currIdx < nodes().size());
  assert(currX < _xCount && currY < _yCount);
  assert(data(currIdx).traversable == true);

  const int xCount = static_cast<int>(_xCount);

  const std::array<int, 8> deltaIdx = {
      (-xCount - 1), -xCount, (-xCount + 1), -1, 1,
      (xCount - 1),  xCount,  (xCount + 1),
  };

  VIPRA::idx adjIdx = 0;
  for ( const int dIdx : deltaIdx ) {
    if ( currIdx + dIdx > _grids.size() || currIdx + dIdx < 0 ) {
      _grids[currIdx].neighbors[adjIdx] = false;
      continue;
    }

    _grids[currIdx].neighbors[adjIdx] = _grids[currIdx + dIdx].traversable;
    ++adjIdx;
  }
}

}  // namespace VIPRA::Goals
