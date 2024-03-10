#pragma once

#include <concepts>
#include <filesystem>

#include "vipra/concepts/module.hpp"
#include "vipra/concepts/not_void.hpp"
#include "vipra/modules.hpp"

#include "vipra/geometry/f3d.hpp"
#include "vipra/types/idx.hpp"
#include "vipra/types/state.hpp"
#include "vipra/types/time.hpp"

namespace VIPRA::Concepts {
/**
 * @brief Checks that a type has a write method
 * 
 * @tparam output_t 
 */
template <typename output_t>
concept can_write = requires(output_t output, std::filesystem::path const& dir) {
  {output.write(dir)};
};

/**
 * @brief Checks that a type can write timestep values to an output
 * 
 * @tparam output_t 
 */
template <typename output_t>
concept can_write_timestep_values = requires(output_t output, char const* key, VIPRA::f3d value) {
  {output.timestep_value(key, VIPRA::timestep{}, value)};
  { output.timestep_value(key, VIPRA::timestep{}, VIPRA::f3d{}) } -> std::same_as<void>;
  { output.timestep_value(key, VIPRA::timestep{}, std::string{}) } -> std::same_as<void>;
  {output.ped_timestep_value(VIPRA::idx{}, VIPRA::timestep{}, key, value)};
  { output.ped_timestep_value(VIPRA::idx{}, VIPRA::timestep{}, key, VIPRA::f3d{}) } -> std::same_as<void>;
  { output.ped_timestep_value(VIPRA::idx{}, VIPRA::timestep{}, key, std::string{}) } -> std::same_as<void>;
};

/**
 * @brief Checks that a type can write simulation values to an output
 * 
 * @tparam output_t 
 */
template <typename output_t>
concept can_write_sim_values = requires(output_t output, char const* key, VIPRA::f3d value) {
  {output.sim_value(key, value)};
  { output.sim_value(key, VIPRA::f3d{}) } -> std::same_as<void>;
  { output.sim_value(key, std::string{}) } -> std::same_as<void>;
};

/**
 * @brief Checks that a type can write pedestrian values to an output
 * 
 * @tparam output_t 
 */
template <typename output_t>
concept can_write_ped_values = requires(output_t output, VIPRA::idx idx, char const* key, VIPRA::f3d value) {
  {output.ped_value(idx, key, value)};
  { output.ped_value(VIPRA::idx{}, key, VIPRA::f3d{}) } -> std::same_as<void>;
  { output.ped_value(VIPRA::idx{}, key, std::string{}) } -> std::same_as<void>;
};

template <typename output_t>
concept BaseOutput =
    can_write_timestep_values<output_t> && can_write_sim_values<output_t> && can_write_ped_values<output_t>;

/**
 * @brief Checks that a type is an output module
 * 
 * @tparam output_t 
 */
template <typename output_t>
concept OutputModule =
    is_module<output_t, VIPRA::Modules::Type::OUTPUT> && can_write<output_t> && BaseOutput<output_t>;

/**
 * @brief Dummy output for use in other concepts
 * 
 */
class DummyOutput {
  // NOLINTBEGIN
 public:
  constexpr static VIPRA::Modules::Type _VIPRA_MODULE_TYPE_ = VIPRA::Modules::Type::OUTPUT;

  using output_data_t = void;

  template <typename params_t>
  void register_params(params_t&) {}

  void config(auto&, VIPRA::Random::Engine&) {}

  void write(std::filesystem::path const&) {}
  void sim_value(char const* /*unused*/, auto&& /*unused*/) {}
  void timestep_value(char const* /*unused*/, VIPRA::timestep, auto&& /*unused*/) {}
  void ped_value(VIPRA::idx /*unused*/, char const* /*unused*/, auto&& /*unused*/) {}
  void ped_timestep_value(VIPRA::idx /*unused*/, VIPRA::timestep, char const* /*unused*/, auto&& /*unused*/) {
  }
  // NOLINTEND
};

CHECK_MODULE(OutputModule, DummyOutput);
}  // namespace VIPRA::Concepts
