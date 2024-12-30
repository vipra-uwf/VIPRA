
/**
 * This is an example parallel parameter sweep simulation that uses the module implementations that come with the VIPRA library.
 * 
 */

#include "vipra.hpp"

auto main(int argc, char** argv) -> int
{
  // Initialize MPI engine
  VIPRA::ParameterSweep::initialize(argc, argv);

  // Register Command Line Arguments
  VIPRA::Args::register_arg("count", "1", VIPRA::ArgType::REQUIRED | VIPRA::ArgType::VALUE_REQUIRED);
  VIPRA::Args::register_arg("map", VIPRA::ArgType::REQUIRED | VIPRA::ArgType::VALUE_REQUIRED);
  VIPRA::Args::register_arg("peds", "", VIPRA::ArgType::REQUIRED | VIPRA::ArgType::VALUE_REQUIRED);
  VIPRA::Args::register_arg("timings", "", VIPRA::ArgType::REQUIRED | VIPRA::ArgType::VALUE_REQUIRED);
  VIPRA::Args::register_arg("params", "module_params.json", VIPRA::ArgType::REQUIRED | VIPRA::ArgType::VALUE_REQUIRED);
  VIPRA::Args::register_arg("modules", "sim_config.json", VIPRA::ArgType::REQUIRED | VIPRA::ArgType::VALUE_REQUIRED);
  VIPRA::Args::register_arg("install", "install", VIPRA::ArgType::REQUIRED | VIPRA::ArgType::VALUE_REQUIRED);
  VIPRA::Args::register_arg("log", "info", VIPRA::ArgType::REQUIRED | VIPRA::ArgType::VALUE_REQUIRED);
  VIPRA::Args::parse(argc, argv);

  // Set the logging level to info, default is INFO
  VIPRA::Log::set_level(VIPRA::Args::get("log"));

  // Create the simulation and load the modules
  VIPRA::Simulation sim;
  sim.set_install_dir(VIPRA::Args::get("install"));
  sim.set_modules(VIPRA::Args::get("modules"));

  // Create a Timer and start it
  VIPRA::Util::Clock<VIPRA::Util::milli> timer{};
  timer.start();

  // Run the parameter sweep
  VIPRA::ParameterSweep::run(
      sim,
      VIPRA::Args::get("peds"),
      VIPRA::Args::get("map"),
      VIPRA::Args::get("params"),
      VIPRA::Args::get("timings"),
      VIPRA::Args::get<size_t>("count"),
      [](VIPRA::idx simId) {  // Callback function called after each simulation run
        VIPRA::Log::info("Simulation id: {} complete on: {}", simId,
                         VIPRA::ParameterSweep::get_rank());
      });

  // Only the master node prints the time taken
  VIPRA::Util::master_do([&]() {
    auto time = timer.stop();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(time);
    time -= seconds;
    auto milliseconds =
        std::chrono::duration_cast<std::chrono::milliseconds>(time);

    VIPRA::Log::info("Total Time taken: {}s {}ms", seconds.count(),
                     milliseconds.count());
    
#ifdef VIPRA_TIME_SIM
    VIPRA::Util::append_to_file(
        VIPRA::Args::get("timings"),
        fmt::format("total,{}s {}ms\n", seconds.count(), milliseconds.count()));
#endif
  });
}