#pragma once

#include <stdexcept>

#include "vipra/geometry/f3d.hpp"
#include "vipra/macros/goals.hpp"
#include "vipra/macros/module.hpp"
#include "vipra/macros/parameters.hpp"

#include "vipra/modules/goals.hpp"
#include "vipra/modules/module.hpp"
#include "vipra/types/float.hpp"

#include "density_grid.hpp"
#include "grid.hpp"

namespace VIPRA::Goals {
class PotentialField : public VIPRA::Modules::Module<PotentialField>,
                       public VIPRA::Modules::Goals {
 public:
  VIPRA_MODULE_NAME("PotentialField")
  VIPRA_MODULE_TYPE(Goals)

  VIPRA_REGISTER_PARAMS(VIPRA_PARAM("endGoalType", _endGoalType),
                        VIPRA_PARAM("cellSize", _cellSize),
                        VIPRA_PARAM("densityUpdateFrequency",
                                    _densityUpdateFrequency),
                        VIPRA_PARAM("densityCellSize", _densityCellSize),
                        VIPRA_PARAM("densityWeight", _densityWeight))

  // NOLINTNEXTLINE(misc-unused-parameters)
  VIPRA_GOALS_INIT_STEP
  {
    _field.intialize(map, _cellSize);
    _densityMap.intialize(map, _densityCellSize);
    fill_grid(map);

    for ( VIPRA::idx pedIdx = 0; pedIdx < pedset.num_pedestrians(); ++pedIdx ) {
      VIPRA::f3d  pos = pedset.ped_coords(pedIdx);
      auto const& grid = _field.get_grid(pos);

      if ( grid.end == _emptyf3d_ )
        VIPRA_MODULE_ERROR(
          "No path found for pedestrian {}, Start: ({}, {}), End: ({}, {})",
          pedIdx, pos.x, pos.y, end_goal(pedIdx).x, end_goal(pedIdx).y);

      set_current_goal(pedIdx, pos + grid.direction);
      set_end_goal(pedIdx, grid.end);
    }
  }

  // NOLINTNEXTLINE(misc-unused-parameters)
  VIPRA_GOALS_UPDATE_STEP
  {
    _densityCheckCounter++;

    // Probably a better way to do this that doesn't reuse code, but this avoids repeating the same check every single loop.
    // Who knows, maybe compiler would have already optimized this.
    if ( _densityCheckCounter >= _densityUpdateFrequency ) {
      fill_grid(map);

      for ( VIPRA::idx pedIdx = 0; pedIdx < pedset.num_pedestrians();
            ++pedIdx ) {
        VIPRA::f3d pos = pedset.ped_coords(pedIdx);
        VIPRA::f3d direction = _field.get_grid(pos).direction;

        set_current_goal(pedIdx, pos + direction);
        update_ped_density(pos);
      }

      _densityMap.clear_grid();

      _densityCheckCounter = 0;
      return;
    }

    for ( VIPRA::idx pedIdx = 0; pedIdx < pedset.num_pedestrians(); ++pedIdx ) {
      VIPRA::f3d pos = pedset.ped_coords(pedIdx);
      VIPRA::f3d direction = _field.get_grid(pos).direction;

      set_current_goal(pedIdx, pos + direction);
    }
  }

  // NOLINTNEXTLINE(misc-unused-parameters)
  VIPRA_GOALS_NEXT_GOAL { return false; }

  // NOLINTNEXTLINE(misc-unused-parameters)
  VIPRA_GOALS_CHANGE_GOAL
  {
    throw std::runtime_error("Potential Field Goals is unable to change goals");
  }

 private:
  // TODO(rolland): update SpatialMap to handle this use case
  Grid        _field;
  DensityGrid _densityMap;

  int _densityCheckCounter;

  VIPRA::f_pnt _cellSize{};
  VIPRA::f_pnt _densityCellSize{};
  VIPRA::f_pnt _densityWeight{};

  std::string _endGoalType;
  int         _densityUpdateFrequency;

  void fill_grid(VIPRA::Modules::Map const& map)
  {  // find the end goals, provided as a module parameter
    auto const& objectives = map.get_objectives(_endGoalType);
    if ( objectives.empty() ) {
      throw std::runtime_error("No objectives of type " + _endGoalType +
                               " found in map");
    }

    for ( auto const& objective : objectives ) {
      _field.flood_fill(objective.center(), map, _densityMap);
    }
  }

  void update_ped_density(VIPRA::f3d const& ped)
  {
    _densityMap.incr_gridpoint(ped);
  }
};
}  // namespace VIPRA::Goals