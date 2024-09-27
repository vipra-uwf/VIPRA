
/**
 * This is an example parallel parameter sweep simulation that uses the module implementations that come with the VIPRA library.
 * 
 */

#include <vipra.hpp>
#include "vipra/logging/logging.hpp"
#include "vipra/simulation/sim_type.hpp"

auto main(int argc, char** argv) -> int
{
  // Set the logging level to info, default is ERROR
  VIPRA::Log::set_level(VIPRA::Log::Level::DEBUG);

  VIPRA::SimType sim;

  VIPRA::Util::Clock<VIPRA::Util::milli> timer{};
  timer.start();

  sim.set_install_dir("/home/rgoodenough/school/VIPRA/install");

  VIPRA::Log::debug("setting Modules");

  sim.set_module(VIPRA::Modules::Type::Model, "Calm");
  sim.set_module(VIPRA::Modules::Type::Goals, "PotentialField");
  sim.set_module(VIPRA::Modules::Type::Map, "QuadTree");
  sim.set_module(VIPRA::Modules::Type::Pedestrians, "SpatialGrid");
  sim.set_module(VIPRA::Modules::Type::Output, "Trajectories");
  sim.set_module(VIPRA::Modules::Type::PedInput, "JSON");
  sim.set_module(VIPRA::Modules::Type::MapInput, "JSON");

  VIPRA::Log::debug("running");

  sim("/home/rgoodenough/school/VIPRA/maps/pedestrians/a320/"
      "a320_144_pedestrians.json",
      "/home/rgoodenough/school/VIPRA/maps/obstacles/a320/a320_polygons.json",
      "/home/rgoodenough/school/VIPRA/examples/module_params.json");

  auto time = timer.stop();
  auto seconds = std::chrono::duration_cast<std::chrono::seconds>(time);
  time -= seconds;
  auto milliseconds =
      std::chrono::duration_cast<std::chrono::milliseconds>(time);

  VIPRA::Log::info("Time taken: {}s {}ms", seconds.count(),
                   milliseconds.count());

  // // Initialize MPI engine
  // VIPRA::ParameterSweep::initialize(argc, argv);

  // // Register Command Line Arguments
  // VIPRA::Args::register_arg("count", "1", VIPRA::ArgType::REQUIRED | VIPRA::ArgType::VALUE_REQUIRED);
  // VIPRA::Args::register_arg("map", VIPRA::ArgType::REQUIRED | VIPRA::ArgType::VALUE_REQUIRED);
  // VIPRA::Args::register_arg("peds", VIPRA::ArgType::REQUIRED | VIPRA::ArgType::VALUE_REQUIRED);
  // VIPRA::Args::register_arg("params", "module_params.json", VIPRA::ArgType::REQUIRED | VIPRA::ArgType::VALUE_REQUIRED);
  // VIPRA::Args::parse(argc, argv);

  // // Create Simulation
  // auto sim = VIPRA::simulation(
  //   Model::calm{},
  //   VIPRA::Goals::PotentialField{},
  //   VIPRA::Pedestrians::Grid{},
  //   VIPRA::CoordModules::Output{
  //     VIPRA::Output::TrajectoriesJson{}
  //   },
  //   VIPRA::Map::QuadTree{}
  // );

  // // Create a Timer and start it
  // VIPRA::Util::Clock<VIPRA::Util::milli> timer{};
  // timer.start();

  // auto params = VIPRA::Parameters{VIPRA::Input::JSON{VIPRA::Args::get("params")}};

  // // Run the parameter sweep
  // VIPRA::ParameterSweep::run(
  //   sim,
  //   VIPRA::Input::JSON{VIPRA::Args::get("peds")}, // Pedestrian Input
  //   VIPRA::Input::DXF{VIPRA::Args::get("map")},  // Map Input
  //   params,
  //   VIPRA::Args::get<size_t>("count"), // Number of simulations to run in total
  //   [](VIPRA::idx simId) {  // Callback function called after each simulation run
  //     VIPRA::Log::info("Simulation id: {} complete on: {}", simId, VIPRA::ParameterSweep::get_rank());
  //   });

  // // Only the master node prints the time taken
  // VIPRA::ParameterSweep::master_do(
  //   [&]() {
  //     auto time = timer.stop();
  //     auto seconds = std::chrono::duration_cast<std::chrono::seconds>(time);
  //     time -= seconds;
  //     auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(time);

  //     VIPRA::Log::info("Time taken: {}s {}ms", seconds.count(), milliseconds.count());
  //   }
  // );
}