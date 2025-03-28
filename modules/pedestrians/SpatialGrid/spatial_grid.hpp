#pragma once

#include <vector>

#include "spatial_map.hpp"
#include "vipra/macros/pedestrians.hpp"
#include "vipra/modules/pedestrians.hpp"

namespace VIPRA::Pedestrians {

/**
 * @brief Pedestrian module that uses a grid to store pedestrians
 * 
 */
NEW_VIPRA_MODULE(SpatialGrid, Pedestrians)
{
 public:
  VIPRA_MODULE_NAME("SpatialGrid");
  VIPRA_MODULE_TYPE(Pedestrians);

  VIPRA_REGISTER_PARAMS(VIPRA_PARAM("gridSize", _cellSize));

  VIPRA_PEDS_INIT_STEP   override;
  VIPRA_PEDS_UPDATE_STEP override;
  VIPRA_PEDS_RESET       override;

  [[nodiscard]] auto conditional_closest_ped(
      VIPRA::idx ped, std::function<bool(VIPRA::idx)> const& condition)
      const -> VIPRA::idx override;
  [[nodiscard]] auto closest_ped(VIPRA::idx ped) const -> VIPRA::idx override;
  [[nodiscard]] auto all_neighbors_within(VIPRA::idx pedIdx, VIPRA::f_pnt radius)
      const -> std::vector<VIPRA::idx> override;

 private:
  VIPRA::f_pnt                                  _cellSize{};
  VIPRA::DataStructures::SpatialMap<VIPRA::idx> _spatialGrid;
};
}  // namespace VIPRA::Pedestrians