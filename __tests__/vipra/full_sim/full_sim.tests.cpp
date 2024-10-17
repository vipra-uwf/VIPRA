
#include <gtest/gtest.h>

#include "vipra.hpp"
#include "vipra/simulation/sim_type.hpp"

TEST(VipraFullSim, FullSim)
{
  VIPRA::Simulation sim{};
  sim.set_install_dir("install");
  sim.set_modules("__tests__/vipra/full_sim/sim_config.json");

  // Run the simulation with the module parameters in 'module_params.json'
  sim("maps/pedestrians/a320/a320_144_pedestrians.json",
      "maps/obstacles/a320/a320_polygons.json",
      "__tests__/vipra/full_sim/module_params.json");
}