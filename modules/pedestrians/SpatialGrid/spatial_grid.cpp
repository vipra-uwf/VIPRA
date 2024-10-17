

#include "spatial_grid.hpp"

#include "vipra/macros/module.hpp"
#include "vipra/modules/pedestrians.hpp"

VIPRA_REGISTER_MODULE(VIPRA::Pedestrians::SpatialGrid, Pedestrians)

namespace VIPRA::Pedestrians {
void SpatialGrid::init_step(VIPRA ::Modules ::Map const& map,
                            VIPRA ::Random ::Engine& /*engine*/)
{
  // create temporary vector of indices to initialize spatial map with
  std::vector<VIPRA::idx> tempIndexes(get_coordinates().size());
  std::iota(std::begin(tempIndexes), std::end(tempIndexes), 0);

  // initialize spatial map
  auto dimensions = map.get_dimensions();
  _spatialGrid.initialize(_cellSize, dimensions.x, dimensions.y,
                          get_coordinates(), tempIndexes);
}

void SpatialGrid::update_step(VIPRA ::State const& state)
{
  // Update pedestrian positions in grids
  _spatialGrid.update_grids(get_coordinates(), state.positions);
}

auto SpatialGrid::conditional_closest_ped(
    VIPRA::idx                             ped,
    std::function<bool(VIPRA::idx)> const& condition) const -> VIPRA::idx
{
  const VIPRA::f3d pos = ped_coords(ped);
  VIPRA::f_pnt     minDist = std::numeric_limits<VIPRA::f_pnt>::max();
  VIPRA::idx       minIdx = ped;

  // Check all surrounding grids for the nearest pedestrian that matches the predicate
  _spatialGrid.for_each_neighbor(pos, [&](VIPRA::idx other) {
    VIPRA::f_pnt dist = pos.distance_to_sqrd(ped_coords(other));

    if ( dist < minDist ) {
      if ( ! condition(other) ) return;

      minDist = dist;
      minIdx = other;
    }
  });

  return minIdx;
}

auto SpatialGrid::closest_ped(VIPRA::idx ped) const -> VIPRA::idx
{
  VIPRA::f_pnt minDist = std::numeric_limits<VIPRA::f_pnt>::max();
  VIPRA::idx   minIdx = ped;

  // Check all surrounding grids for the nearest pedestrian that matches the predicate
  _spatialGrid.for_each_neighbor(ped_coords(ped), [&](VIPRA::idx other) {
    VIPRA::f_pnt dist = ped_coords(ped).distance_to_sqrd(ped_coords(other));

    if ( dist < minDist ) {
      minDist = dist;
      minIdx = other;
    }
  });

  return minIdx;
}

auto SpatialGrid::all_neighbors_within(
    VIPRA::idx pedIdx, VIPRA::f_pnt radius) const -> std::vector<VIPRA::idx>
{
  std::vector<VIPRA::idx> neighbors;

  const VIPRA::f_pnt rad2 = radius * radius;

  _spatialGrid.for_each_neighbor(ped_coords(pedIdx), [&](VIPRA::idx other) {
    VIPRA::f_pnt dist = ped_coords(pedIdx).distance_to_sqrd(ped_coords(other));

    if ( dist <= rad2 ) {
      neighbors.push_back(other);
    }
  });

  return neighbors;
}
}  // namespace VIPRA::Pedestrians