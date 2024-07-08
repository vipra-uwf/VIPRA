
/**
 * This is an example parallel parameter sweep simulation that uses the module implementations that come with the VIPRA library.
 * 
 */

#include <iostream>

#include <vipra.hpp>

#include "modules/model/calm_model/calm_model.hpp"

auto main(int argc, char** argv) -> int {
  // Initialize MPI engine
  VIPRA::ParameterSweep::initialize(argc, argv);

  // Get sim count from arguments
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <simCount>" << std::endl;
    return 1;
  }
  size_t simCount = std::stoul(argv[1]);
  

  // Create Simulation
  auto sim = VIPRA::simulation(
      CALM::Model{},
      VIPRA::Goals::AStar{},
      VIPRA::Pedestrians::Grid{},
      VIPRA::CoordModules::Output{
        VIPRA::Output::Trajectories::JSON{}
      },
      VIPRA::Obstacles::QuadTree{}
  );

  // Create a Timer and start it
  VIPRA::Util::Clock<VIPRA::Util::milli> timer{};
  timer.start();

  // Run the parameter sweep
  VIPRA::ParameterSweep::run(
      sim,
      VIPRA::Input::JSON{"maps/pedestrians/a320/a320_144_pedestrians.json"},
      VIPRA::Input::JSON{"maps/obstacles/a320/a320_polygons.json"},
      VIPRA::Parameters{
        VIPRA::Input::JSON{"examples/module_params.json"}
      },
      simCount,
      [](VIPRA::idx simId) { std::cout << "Simulation id: " << simId << " complete on: " << VIPRA::ParameterSweep::get_rank() << std::endl; });

  // Only the master node prints the time taken
  VIPRA::ParameterSweep::master_do([&](){
    auto time = timer.stop();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(time);
    time -= seconds;
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(time);

    std::cout << "Time taken: " << seconds.count() << "s " << milliseconds.count() << "ms" << std::endl;
    VIPRA_OUTPUT_PERFORMANCE;
  });
}
