
## Example Parameter Sweep

This example uses the parameter sweep engine.

It takes in the total number of simulations to run as a command line argument.

```C++
#include <vipra.hpp>
#include "modules/model/calm_model/calm_model.hpp"

int main(int argc, char** argv)
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
  auto sim = VIPRA::simulation(
    CALM::Model{},                        // Pedestrian Model
    VIPRA::Goals::AStar{},                // Goals Module
    VIPRA::Pedestrians::Grid{},           // Pedestrian Set
    VIPRA::CoordModules::Output {         // Output Coordinator
      VIPRA::Output::Trajectories::JSON{} // Output Module
    },
    VIPRA::Obstacles::QuadTree{}          // Obstacle Set
  );

  // Run the parameter sweep
  VIPRA::ParameterSweep::run(
    sim,                                                                      // Simulation Object
    VIPRA::Input::JSON{"maps/pedestrians/a320/a320_144_pedestrians.json"},    // Pedestrian Input
    VIPRA::Input::JSON{"maps/obstacles/a320/a320_polygons.json"},             // Obstacle Input
    VIPRA::Parameters{VIPRA::Input::JSON{"examples/module_params.json"}}, simCount, // Parameter Input
    [](VIPRA::idx simId) { // Optional callback function when each simulation run completes
      std::cout << "Simulation id: " << simId 
                << " complete on: "  << VIPRA::ParameterSweep::get_rank() << std::endl;
    });
}
```