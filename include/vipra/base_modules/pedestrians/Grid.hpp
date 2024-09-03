#pragma once

#include <iostream>
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
#include "vipra/types/size.hpp"
#include "vipra/types/state.hpp"
#include "vipra/types/util/result_or_void.hpp"

namespace VIPRA::Pedestrians {

/**
 * @brief Pedestrian module that uses a grid to store pedestrians
 * 
 */
class Grid : public Modules::Module<Grid>, public Modules::Pedestrians<Grid> {
 public:
  VIPRA_MODULE_NAME("Grid");
  VIPRA_MODULE_TYPE(Pedestrians);

  VIPRA_REGISTER_PARAMS(VIPRA_PARAM("gridSize", _cellSize))

  VIPRA_PEDS_INIT_STEP
  {
    auto coords = input.template get<std::vector<VIPRA::f3d>>({"coords"});
    if ( ! coords ) throw std::runtime_error("Could not find pedestrian coordinates in input file");

    auto dimensions = obstacles.get_dimensions();

    // create temporary vector of indices to initialize spatial map with
    std::vector<VIPRA::idx> tempIndexes((*coords).size());
    std::iota(std::begin(tempIndexes), std::end(tempIndexes), 0);

    _spatialGrid.initialize(_cellSize, dimensions.x, dimensions.y, *coords, tempIndexes);

    set_velocities(std::vector<VIPRA::f3d>((*coords).size()));
    set_coordinates(std::move(*coords));
  }

  VIPRA_PEDS_UPDATE_STEP
  {
    // Update pedestrian positions in grids
    _spatialGrid.update_grids(get_coordinates(), state.positions);
  }

  VIPRA_PERF_FUNC [[nodiscard]] auto conditional_closest_ped_impl(VIPRA::idx ped,
                                                                  auto&&     condition) const -> VIPRA::idx
  {
    VIPRA::f_pnt minDist = std::numeric_limits<VIPRA::f_pnt>::max();
    VIPRA::idx   minIdx = ped;

    // Check all surrounding grids for the nearest pedestrian that matches the predicate
    _spatialGrid.for_each_neighbor(ped_coords(ped), [&](VIPRA::idx other) VIPRA_PERF_FUNC {
      VIPRA::f_pnt dist = distance(ped, other);

      if ( dist < minDist ) {
        if ( ! condition(other) ) return;

        minDist = dist;
        minIdx = other;
      }
    });

    return minIdx;
  }

  VIPRA_PERF_FUNC [[nodiscard]] auto closest_ped_impl(VIPRA::idx ped) const -> VIPRA::idx
  {
    VIPRA::f_pnt minDist = std::numeric_limits<VIPRA::f_pnt>::max();
    VIPRA::idx   minIdx = ped;

    // Check all surrounding grids for the nearest pedestrian that matches the predicate
    _spatialGrid.for_each_neighbor(ped_coords(ped), [&](VIPRA::idx other) VIPRA_PERF_FUNC {
      VIPRA::f_pnt dist = ped_coords(ped).distance_to(ped_coords(other));

      if ( dist < minDist ) {
        minDist = dist;
        minIdx = other;
      }
    });

    return minIdx;
  }

  VIPRA_PERF_FUNC [[nodiscard]] auto all_neighbors_within_impl(VIPRA::idx pedIdx, VIPRA::f_pnt radius) const
      -> std::vector<VIPRA::idx>
  {
    std::vector<VIPRA::idx> neighbors;

    _spatialGrid.for_each_neighbor(ped_coords(pedIdx), [&](VIPRA::idx other) VIPRA_PERF_FUNC {
      VIPRA::f_pnt dist = ped_coords(pedIdx).distance_to(ped_coords(other));

      if ( dist <= radius ) {
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