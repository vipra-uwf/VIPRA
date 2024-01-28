#pragma once

#include <filesystem>
#include <tuple>
#include <type_traits>
#include "vipra/concepts/output.hpp"
#include "vipra/concepts/output_coordinator.hpp"
#include "vipra/types/idx.hpp"
#include "vipra/types/util/result_or_void.hpp"

namespace VIPRA::Module {
template <Concepts::OutputModule... output_ts>
class Output {
  // TODO(rolland): decide if we need std::remove_reference
  // TODO(rolland): need to figure out how to get paths for each output
  //                   - if multiple output modules use the same parameter, how do we split them up
  //                   - maybe require a path parameter for each output module, in their constructor?
  //                            - this would require a recompile for changing paths

  // TODO(rolland): when running a parameter sweep switch output directory to a new directory for each run

  template <typename output_t>
  // NOLINTNEXTLINE(readability-identifier-naming) helper struct
  struct write_helper {
    static auto write(output_t& output, const std::filesystem::path& dir) {
      if constexpr (std::is_same_v<result_or_VOID_t<decltype(std::declval<output_t>().write(dir))>, VOID>) {
        output.write(dir);
        return VOID{};
      } else {
        return output.write(dir);
      }
    }
  };

 public:
  // NOLINTNEXTLINE
  constexpr static VIPRA::Modules::Type _VIPRA_MODULE_TYPE_ = VIPRA::Modules::Type::OUTPUT;

  constexpr explicit Output(output_ts... outputs) : _outputs(std::make_tuple(outputs...)) {}

  void new_run(VIPRA::idx runIdx) {
    _current_output_dir = _base_output_dir / std::to_string(runIdx);
    create_output_directory(_current_output_dir);
  }

  /**
   * @brief Calls write on all outputs, returning a tuple of the results or VOID
   * 
   * @return result_or_VOID_tuple<std::tuple<output_ts...>>::type 
   */
  auto write() -> std::tuple<
      result_or_VOID_t<decltype(std::declval<output_ts>().write(std::declval<std::filesystem::path>()))>...> {
    return std::apply(
        [&](auto&&... outputs) {
          return std::make_tuple(write_helper<decltype(outputs)>::write(outputs, _current_output_dir)...);
        },
        _outputs);
  }

  /**
   * @brief Calls config on all output modules
   * 
   * @param params 
   */
  void config(const auto& params) {
    _base_output_dir =
        params.template get_param<std::string>(VIPRA::Modules::Type::OUTPUT, "coordinator", "output_dir");
    _current_output_dir = _base_output_dir;

    create_output_directory(_current_output_dir);
    std::apply([&params](auto&&... outputs) { (outputs.config(params), ...); }, _outputs);
  }

  /**
   * @brief Calls register_params on all output modules
   * 
   * @tparam params_t 
   */
  template <Concepts::ParamModule params_t>
  void register_params(params_t& params) {
    params.register_param(VIPRA::Modules::Type::OUTPUT, "coordinator", "output_dir", ParameterType::REQUIRED);
    std::apply([&](auto&&... outputs) { (outputs.template register_params<params_t>(params), ...); },
               _outputs);
  }

  /**
   * @brief Calls sim_value on all output modules
   * 
   * @param key 
   * @param value 
   */
  void sim_value(const char* key, auto&& value) {
    std::apply([&key, &value](auto&&... outputs) { (outputs.sim_value(key, value), ...); }, _outputs);
  }

  /**
   * @brief Calls timestep_value on all output modules
   * 
   * @param key 
   * @param value 
   */
  void timestep_value(const char* key, VIPRA::timestep timestep, auto&& value) {
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
  void ped_value(VIPRA::idx pedIdx, const char* key, auto&& value) {
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
  void ped_timestep_value(VIPRA::idx pedIdx, VIPRA::timestep timestep, const char* key, auto&& value) {
    std::apply([&pedIdx, &timestep, &key, &value](
                   auto&&... outputs) { (outputs.ped_timestep_value(pedIdx, timestep, key, value), ...); },
               _outputs);
  }

 private:
  std::tuple<output_ts...> _outputs;

  std::filesystem::path _base_output_dir;
  std::filesystem::path _current_output_dir;

  static void create_output_directory(const std::filesystem::path& directory) {
    if (std::filesystem::exists(directory)) {
      if (std::filesystem::is_directory(directory)) {
        return;
      }

      throw std::runtime_error("Output directory already exists and is not a directory: " +
                               directory.string());
    }

    if (!std::filesystem::create_directory(directory)) {
      throw std::runtime_error("Could not create output directory: " + directory.string());
    }
  }
};

CHECK_MODULE(OutputCoordinator, Output<Concepts::DummyOutput>);
}  // namespace VIPRA::Module
