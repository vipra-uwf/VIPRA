#pragma once

#include <numeric>
#include <vector>

#include "vipra/data_structures/spatial_map.hpp"

#include "vipra/macros/module.hpp"
#include "vipra/macros/parameters.hpp"
#include "vipra/macros/pedestrians.hpp"
#include "vipra/macros/performance.hpp"

#include "vipra/modules/module.hpp"
#include "vipra/modules/pedestrians.hpp"

#include "vipra/geometry/f3d.hpp"

#include "vipra/types/float.hpp"
#include "vipra/types/idx.hpp"

namespace VIPRA::Pedestrians {

/**
 * @brief Pedestrian module that uses a grid to store pedestrians
 * 
 */
class SpatialGrid : public VIPRA::Modules::Module<SpatialGrid>,
                    public VIPRA::Modules::Pedestrians {
 public:
  VIPRA_MODULE_NAME("SpatialGrid");
  VIPRA_MODULE_TYPE(Pedestrians);

  VIPRA_REGISTER_PARAMS(VIPRA_PARAM("gridSize", _cellSize))

  // NOLINTNEXTLINE(misc-unused-parameters)
  VIPRA_PEDS_INIT_STEP override
  {
    // create temporary vector of indices to initialize spatial map with
    std::vector<VIPRA::idx> tempIndexes(get_coordinates().size());
    std::iota(std::begin(tempIndexes), std::end(tempIndexes), 0);

    // initialize spatial map
    auto dimensions = map.get_dimensions();
    _spatialGrid.initialize(_cellSize, dimensions.x, dimensions.y,
                            get_coordinates(), tempIndexes);
  }

  VIPRA_PEDS_UPDATE_STEP override
  {
    // Update pedestrian positions in grids
    _spatialGrid.update_grids(get_coordinates(), state.positions);
  }

  [[nodiscard]] VIPRA_PERF_FUNC auto conditional_closest_ped(
      VIPRA::idx ped, std::function<bool(VIPRA::idx)> const& condition) const
      -> VIPRA::idx override
  {
    const VIPRA::f3d pos = ped_coords(ped);
    VIPRA::f_pnt     minDist = std::numeric_limits<VIPRA::f_pnt>::max();
    VIPRA::idx       minIdx = ped;

    // Check all surrounding grids for the nearest pedestrian that matches the predicate
    _spatialGrid.for_each_neighbor(pos, [&](VIPRA::idx other) VIPRA_PERF_FUNC {
      VIPRA::f_pnt dist = pos.distance_to_sqrd(ped_coords(other));

      if ( dist < minDist ) {
        if ( ! condition(other) ) return;

        minDist = dist;
        minIdx = other;
      }
    });

    return minIdx;
  }

  [[nodiscard]] VIPRA_PERF_FUNC auto closest_ped(VIPRA::idx ped) const
      -> VIPRA::idx override
  {
    VIPRA::f_pnt minDist = std::numeric_limits<VIPRA::f_pnt>::max();
    VIPRA::idx   minIdx = ped;

    // Check all surrounding grids for the nearest pedestrian that matches the predicate
    _spatialGrid.for_each_neighbor(
        ped_coords(ped), [&](VIPRA::idx other) VIPRA_PERF_FUNC {
          VIPRA::f_pnt dist =
              ped_coords(ped).distance_to_sqrd(ped_coords(other));

          if ( dist < minDist ) {
            minDist = dist;
            minIdx = other;
          }
        });

    return minIdx;
  }

  [[nodiscard]] VIPRA_PERF_FUNC auto all_neighbors_within(
      VIPRA::idx   pedIdx,
      VIPRA::f_pnt radius) const -> std::vector<VIPRA::idx> override
  {
    std::vector<VIPRA::idx> neighbors;

    const VIPRA::f_pnt rad2 = radius * radius;

    _spatialGrid.for_each_neighbor(
        ped_coords(pedIdx), [&](VIPRA::idx other) VIPRA_PERF_FUNC {
          VIPRA::f_pnt dist =
              ped_coords(pedIdx).distance_to_sqrd(ped_coords(other));

          if ( dist <= rad2 ) {
            neighbors.push_back(other);
          }
        });

    return neighbors;
  }

 private:
  VIPRA::f_pnt _cellSize{};

  VIPRA::DataStructures::SpatialMap<VIPRA::idx> _spatialGrid;
};
}  // namespace VIPRA::Pedestrians