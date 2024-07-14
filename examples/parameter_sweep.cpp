
/**
 * This is an example parallel parameter sweep simulation that uses the module implementations that come with the VIPRA library.
 * 
 */

#include <vipra.hpp>

#include "modules/model/calm_model/calm_model.hpp"

auto main(int argc, char** argv) -> int
{
  // Initialize MPI engine
  VIPRA::ParameterSweep::initialize(argc, argv);

  // Get sim count from arguments
  if ( argc != 2 ) {
    std::cerr << "Usage: " << argv[0] << " <simCount>" << std::endl;
    return 1;
  }
  size_t simCount = std::stoul(argv[1]);

  // Create Simulation
  auto sim = VIPRA::simulation(CALM::Model{}, VIPRA::Goals::AStar{}, VIPRA::Pedestrians::Grid{},
                               VIPRA::CoordModules::Output{VIPRA::Output::Trajectories::JSON{}},
                               VIPRA::Obstacles::QuadTree{});

  // Run the parameter sweep
  VIPRA::ParameterSweep::run(
      sim, VIPRA::Input::JSON{"maps/pedestrians/a320/a320_144_pedestrians.json"},
      VIPRA::Input::JSON{"maps/obstacles/a320/a320_polygons.json"},
      VIPRA::Parameters{VIPRA::Input::JSON{"examples/module_params.json"}}, simCount, [](VIPRA::idx simId) {
        std::cout << "Simulation id: " << simId << " complete on: " << VIPRA::ParameterSweep::get_rank()
                  << std::endl;
      });
}
