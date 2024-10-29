#pragma once

#include <algorithm>
#include <filesystem>
#include <memory>

#include "vipra/macros/module.hpp"
#include "vipra/macros/parameters.hpp"

#include "vipra/modules/module.hpp"
#include "vipra/modules/output.hpp"

#include "vipra/random/random.hpp"

#include "vipra/special_modules/parameters.hpp"
#include "vipra/types/idx.hpp"

// TODO(rolland): add a write to module method, that writes to a module if loaded, does nothing if not

namespace VIPRA::CoordModules {

class OutputCoordinator : public Modules::Module<OutputCoordinator> {
  // TODO(rolland): need to figure out how to get paths for each output
  //                   - if multiple output modules use the same parameter, how do we split them up
  //                   - maybe require a path parameter for each output module, in their constructor?
  //                            - this would require a recompile for changing paths

 public:
  VIPRA_MODULE_NAME("coordinator");
  VIPRA_MODULE_TYPE(Output);

  VIPRA_REGISTER_PARAMS(VIPRA_PARAM("output_dir", _base_output_dir))

  void add_output(
      std::unique_ptr<Modules::Output>&&                                module,
      std::function<void(void*, Parameters&, VIPRA::Random::Engine&)>&& config)
  {
    _outputs.emplace_back(std::move(module));
    _configs.emplace_back(std::move(config));
  }

  void config(Parameters& paramIn, VIPRA::Random::Engine& engine)
  {
    paramIn.register_param(module_type(), module_name(), "output_dir");
    _base_output_dir = paramIn.get_param<std::string>(
        module_type(), module_name(), "output_dir", engine);

    _current_output_dir = _base_output_dir;
    create_output_directory(_current_output_dir);

    for ( size_t i = 0; i < _outputs.size(); ++i ) {
      _configs[i](_outputs[i].get(), paramIn, engine);
    }
  }

  /**
   * @brief Creates a new output directory for the current simulation run
   * 
   * @param runIdx 
   */
  void new_run(VIPRA::idx runIdx)
  {
    _current_output_dir = _base_output_dir / std::to_string(runIdx);
    create_output_directory(_current_output_dir);
  }

  /**
   * @brief Calls write on all outputs, returning a tuple of the results or VOID
   * 
   * @return Util::result_or_VOID_tuple<std::tuple<output_ts...>>::type 
   */
  void write()
  {
    std::for_each(_outputs.begin(), _outputs.end(),
                  [&](auto& output) { output->write(_current_output_dir); });
  }

  void timestep_update(VIPRA::timestep timestep, VIPRA::delta_t timestepSize,
                       VIPRA::State const& state)
  {
    std::for_each(_outputs.begin(), _outputs.end(), [&](auto& output) {
      output->timestep_update(timestep, timestepSize, state);
    });
  }

  /**
   * @brief Writes a string to a file in the current output directory
   * 
   * @param filename 
   * @param value 
   */
  void write_to_file(std::string const& filename, std::string const& value)
  {
    std::filesystem::path filepath = _current_output_dir / filename;
    std::ofstream         file(filepath);

    if ( ! file.is_open() ) {
      VIPRA_MODULE_ERROR("Could not open file for writing: {}",
                         filepath.string());
    }

    file << value;

    file.close();
  }

 private:
  std::vector<std::unique_ptr<VIPRA::Modules::Output>> _outputs;
  std::vector<std::function<void(void*, Parameters&, VIPRA::Random::Engine&)>>
      _configs;

  std::filesystem::path _base_output_dir;
  std::filesystem::path _current_output_dir;

  void create_output_directory(std::filesystem::path const& directory) const
  {
    if ( std::filesystem::exists(directory) ) {
      if ( std::filesystem::is_directory(directory) ) {
        // directory exists and is actually a directory, all is good
        return;
      }

      // exists but isn't a directory, error
      VIPRA_MODULE_ERROR(
          "Output directory already exists and is not a directory: {}",
          directory.string());
    }

    // create and check it was actually created
    if ( ! std::filesystem::create_directory(directory) ) {
      if ( ! std::filesystem::exists(directory) )
        VIPRA_MODULE_ERROR("Could not create output directory: {}",
                           directory.string());
    }
  }
};
}  // namespace VIPRA::CoordModules
