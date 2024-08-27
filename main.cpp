
/**
 * This is an example parallel parameter sweep simulation that uses the module implementations that come with the VIPRA library.
 * 
 */

#include <vipra.hpp>

#include "modules/model/calm_model/calm_model.hpp"
#include "vipra/logging/logging.hpp"
#include "vipra/util/cli_arguments.hpp"

auto main(int argc, char** argv) -> int
{
  // Set the logging level to info, default is ERROR
  VIPRA::Log::set_level(VIPRA::Log::Level::INFO);

  // Initialize MPI engine
  VIPRA::ParameterSweep::initialize(argc, argv);

  // Register Command Line Arguments
  VIPRA::Args::register_arg("count", "1", VIPRA::ArgType::REQUIRED | VIPRA::ArgType::VALUE_REQUIRED);
  VIPRA::Args::register_arg("map", VIPRA::ArgType::REQUIRED | VIPRA::ArgType::VALUE_REQUIRED);
  VIPRA::Args::register_arg("peds", VIPRA::ArgType::REQUIRED | VIPRA::ArgType::VALUE_REQUIRED);
  VIPRA::Args::register_arg("params", "module_params.json", VIPRA::ArgType::REQUIRED | VIPRA::ArgType::VALUE_REQUIRED);
  VIPRA::Args::parse(argc, argv);

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
    VIPRA::Input::JSON{VIPRA::Args::get("peds")},
    VIPRA::Input::JSON{VIPRA::Args::get("map")},
    VIPRA::Parameters{VIPRA::Input::JSON{VIPRA::Args::get("params")}},
    VIPRA::Args::get<size_t>("count"),
    [](VIPRA::idx simId) {
      VIPRA::Log::info("Simulation id: {} complete on: {}", simId, VIPRA::ParameterSweep::get_rank());
    });

  // Only the master node prints the time taken
  VIPRA::ParameterSweep::master_do(
    [&]() {
      auto time = timer.stop();
      auto seconds = std::chrono::duration_cast<std::chrono::seconds>(time);
      time -= seconds;
      auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(time);

      VIPRA::Log::info("Time taken: {}s {}ms", seconds.count(), milliseconds.count());
    }
  );
}