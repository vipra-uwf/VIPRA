#pragma once

#include <filesystem>
#include <tuple>
#include <type_traits>

#include "vipra/macros/module.hpp"
#include "vipra/macros/parameters.hpp"

#include "vipra/modules/module.hpp"
#include "vipra/modules/output.hpp"

#include "vipra/random/random.hpp"

#include "vipra/types/idx.hpp"
#include "vipra/types/time.hpp"
#include "vipra/types/util/result_or_void.hpp"

// TODO(rolland): add a write to module method, that writes to a module if loaded, does nothing if not

namespace VIPRA::CoordModules {
template <typename... output_ts>
class Output : public Modules::Module<Output<output_ts...>>, public Modules::Output<Output<output_ts...>> {
  // TODO(rolland): need to figure out how to get paths for each output
  //                   - if multiple output modules use the same parameter, how do we split them up
  //                   - maybe require a path parameter for each output module, in their constructor?
  //                            - this would require a recompile for changing paths

  template <typename output_t>
  // NOLINTNEXTLINE(readability-identifier-naming) helper struct
  struct write_helper {
    static auto write(output_t& output, std::filesystem::path const& dir)
    {
      if constexpr ( std::is_same_v<Util::result_or_VOID_t<decltype(std::declval<output_t>().write(dir))>,
                                    VOID> ) {
        output.write(dir);
        return VOID{};
      }
      else {
        return output.write(dir);
      }
    }
  };

 public:
  VIPRA_MODULE_NAME("coordinator");
  VIPRA_MODULE_TYPE(Output);

  VIPRA_REGISTER_PARAMS(VIPRA_PARAM("output_dir", _base_output_dir))

  /**
   * @brief Registers the modules parameters with the parameter reader
   * 
   * @tparam paramreader_t 
   * @param paramIn
   */
  template <typename paramreader_t>
  void register_params(paramreader_t&& paramIn)
  {
    Modules::Module<Output<output_ts...>>::register_params(std::forward<paramreader_t>(paramIn));

    std::apply([&](auto&&... outputs) { (outputs.register_params(paramIn), ...); }, _outputs);
  }

  template <typename paramreader_t>
  void config(paramreader_t& paramIn, VIPRA::Random::Engine& engine)
  {
    _base_output_dir =
        paramIn.template get_param<std::string>(module_type(), module_name(), "output_dir", engine);

    _current_output_dir = _base_output_dir;
    create_output_directory(_current_output_dir);

    // configure all outputs
    std::apply([&](auto&&... outputs) { (outputs.config(paramIn, engine), ...); }, _outputs);
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
  auto write() -> std::tuple<Util::result_or_VOID_t<
                   decltype(std::declval<output_ts>().write(std::declval<std::filesystem::path>()))>...>
  {
    return std::apply(
        [&](auto&&... outputs) {
          return std::make_tuple(write_helper<decltype(outputs)>::write(outputs, _current_output_dir)...);
        },
        _outputs);
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
      throw std::runtime_error("Could not open file for writing: " + filepath.string());
    }

    file << value;

    file.close();
  }

  /**
   * @brief Calls sim_value on all output modules
   * 
   * @param key 
   * @param value 
   */
  void sim_value(std::string const& key, auto&& value)
  {
    std::apply([&key, &value](auto&&... outputs) { (outputs.sim_value(key, value), ...); }, _outputs);
  }

  /**
   * @brief Calls timestep_value on all output modules
   * 
   * @param key 
   * @param value 
   */
  void timestep_value(std::string const& key, VIPRA::timestep timestep, auto&& value)
  {
    std::apply(
        [&key, &timestep, &value](auto&&... outputs) { (outputs.timestep_value(key, timestep, value), ...); },
        _outputs);
  }

  /**
   * @brief Calls ped_value on all output modules
   * 
   * @param pedIdx 
   * @param key 
   * @param value 
   */
  void ped_value(VIPRA::idx pedIdx, std::string const& key, auto&& value)
  {
    std::apply([&pedIdx, &key, &value](auto&&... outputs) { (outputs.ped_value(pedIdx, key, value), ...); },
               _outputs);
  }

  /**
   * @brief Calls ped_timestep_value on all output modules
   * 
   * @param pedIdx 
   * @param key 
   * @param value 
   */
  void ped_timestep_value(VIPRA::idx pedIdx, VIPRA::timestep timestep, std::string const& key, auto&& value)
  {
    std::apply([&pedIdx, &timestep, &key, &value](
                   auto&&... outputs) { (outputs.ped_timestep_value(pedIdx, timestep, key, value), ...); },
               _outputs);
  }

 private:
  std::tuple<output_ts...> _outputs;

  std::filesystem::path _base_output_dir;
  std::filesystem::path _current_output_dir;

  static void create_output_directory(std::filesystem::path const& directory)
  {
    if ( std::filesystem::exists(directory) ) {
      if ( std::filesystem::is_directory(directory) ) {
        // directory exists and is actually a directory, all is good
        return;
      }

      // exists but isn't a directory, error
      throw std::runtime_error("Output directory already exists and is not a directory: " +
                               directory.string());
    }

    // create and check it was actually created
    if ( ! std::filesystem::create_directory(directory) ) {
      if ( ! std::filesystem::exists(directory) )
        throw std::runtime_error("Could not create output directory: " + directory.string());
    }
  }

 public:
  constexpr explicit Output(output_ts... outputs) : _outputs(std::make_tuple(outputs...)) {}
};

}  // namespace VIPRA::CoordModules
