#pragma once

#include <stdexcept>

#include "vipra/macros/goals.hpp"
#include "vipra/macros/module.hpp"
#include "vipra/macros/parameters.hpp"

#include "vipra/modules/goals.hpp"
#include "vipra/types/float.hpp"

#include "pathing/grid.hpp"

namespace VIPRA {
NEW_VIPRA_MODULE(PotentialField, Goals)
{
 public:
  VIPRA_MODULE_NAME("PotentialField");
  VIPRA_MODULE_TYPE(Goals);

  VIPRA_REGISTER_PARAMS(VIPRA_PARAM("endGoalType", _endGoalType), VIPRA_PARAM("cellSize", _cellSize))

  // NOLINTNEXTLINE(misc-unused-parameters)
  VIPRA_GOALS_INIT_STEP
  {
    _field.intialize(map, _cellSize);
    fill_grid(map);

    for ( VIPRA::idx pedIdx = 0; pedIdx < pedset.num_pedestrians(); ++pedIdx ) {
      VIPRA::f3d  pos = pedset.ped_coords(pedIdx);
      auto const& grid = _field.get_grid(pos);

      set_current_goal(pedIdx, pos + grid.direction);
      set_end_goal(pedIdx, grid.end);
    }
  }

  // NOLINTNEXTLINE(misc-unused-parameters)
  VIPRA_GOALS_UPDATE_STEP
  {
    for ( VIPRA::idx pedIdx = 0; pedIdx < pedset.num_pedestrians(); ++pedIdx ) {
      VIPRA::f3d pos = pedset.ped_coords(pedIdx);
      VIPRA::f3d direction = _field.get_grid(pos).direction;

      set_current_goal(pedIdx, pos + direction);
    }
  }

  // NOLINTNEXTLINE(misc-unused-parameters)
  VIPRA_GOALS_NEXT_GOAL { return false; }

  // NOLINTNEXTLINE(misc-unused-parameters)
  VIPRA_GOALS_CHANGE_GOAL { throw std::runtime_error("Potential Field Goals is unable to change goals"); }

 private:
  // TODO(rolland): update SpatialMap to handle this use case
  Grid _field;

  VIPRA::f_pnt _cellSize{};
  std::string  _endGoalType;

  void fill_grid(auto const& map)
  {  // find the end goals, provided as a module parameter
    auto const& objectives = map.get_objectives(_endGoalType);
    if ( objectives.empty() ) {
      throw std::runtime_error("No objectives of type " + _endGoalType + " found in map");
    }

    for ( auto const& objective : objectives ) {
      _field.flood_fill(objective.center(), map);
    }
  }
};
}
}  // namespace VIPRA