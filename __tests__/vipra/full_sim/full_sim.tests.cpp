
#include <gtest/gtest.h>

#include "model/calm_model/calm_model.hpp"
#include "vipra.hpp"

TEST(VipraFullSim, FullSim)
{
  auto sim = VIPRA::simulation(
      CALM::Model{},
      VIPRA::Goals::AStar{},
      VIPRA::Pedestrians::Grid{},
      VIPRA::CoordModules::Output{
        VIPRA::Output::Trajectories::JSON{}
      },
      VIPRA::Obstacles::QuadTree{}
  );

  // Run the simulation with the module parameters in 'module_params.json'
  sim(
    VIPRA::Input::JSON{"../maps/pedestrians/a320/a320_144_pedestrians.json"},
    VIPRA::Input::JSON{"../maps/obstacles/a320/a320_polygons.json"},
    VIPRA::Parameters{VIPRA::Input::JSON{"vipra/full_sim/module_params.json"}}
    );
}