
/**
 * This is an example simulation that uses the module implementations that come with the VIPRA library.
 * 
 */

#include <vipra.hpp>

#include "modules/model/calm_model/calm_model.hpp"

auto main() -> int
{
  // Create Simulation
  auto sim = VIPRA::simulation(CALM::Model{}, VIPRA::Goals::AStar{}, VIPRA::Pedestrians::Grid{},
                               VIPRA::CoordModules::Output{VIPRA::Output::Trajectories::JSON{}},
                               VIPRA::Map::QuadTree{});

  // Run the simulation with the module parameters in 'module_params.json'
  sim(VIPRA::Input::JSON{"maps/pedestrians/a320/a320_144_pedestrians.json"},
      VIPRA::Input::JSON{"maps/obstacles/a320/a320_polygons.json"},
      VIPRA::Parameters{VIPRA::Input::JSON{"module_params.json"}});
}