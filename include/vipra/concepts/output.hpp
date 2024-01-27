#pragma once

#include <concepts>

#include "vipra/concepts/module.hpp"
#include "vipra/concepts/not_void.hpp"
#include "vipra/modules.hpp"

#include "vipra/types/f3d.hpp"
#include "vipra/types/idx.hpp"
#include "vipra/types/state.hpp"
#include "vipra/types/time.hpp"

namespace VIPRA::Concepts {

template <typename output_t>
concept can_write = requires(output_t output) {
  {output.write()};
};

template <typename output_t>
concept can_write_timestep_values = requires(output_t output, const char* key, VIPRA::f3d value) {
  {output.timestep_value(key, VIPRA::timestep{}, value)};
  { output.timestep_value(key, VIPRA::timestep{}, VIPRA::f3d{}) } -> std::same_as<void>;
  { output.timestep_value(key, VIPRA::timestep{}, std::string{}) } -> std::same_as<void>;
  {output.ped_timestep_value(VIPRA::idx{}, VIPRA::timestep{}, key, value)};
  { output.ped_timestep_value(VIPRA::idx{}, VIPRA::timestep{}, key, VIPRA::f3d{}) } -> std::same_as<void>;
  { output.ped_timestep_value(VIPRA::idx{}, VIPRA::timestep{}, key, std::string{}) } -> std::same_as<void>;
};

template <typename output_t>
concept can_write_sim_values = requires(output_t output, const char* key, VIPRA::f3d value) {
  {output.sim_value(key, value)};
  { output.sim_value(key, VIPRA::f3d{}) } -> std::same_as<void>;
  { output.sim_value(key, std::string{}) } -> std::same_as<void>;
};

template <typename output_t>
concept can_write_ped_values = requires(output_t output, VIPRA::idx idx, const char* key, VIPRA::f3d value) {
  {output.ped_value(idx, key, value)};
  { output.ped_value(VIPRA::idx{}, key, VIPRA::f3d{}) } -> std::same_as<void>;
  { output.ped_value(VIPRA::idx{}, key, std::string{}) } -> std::same_as<void>;
};

template <typename output_t>
concept OutputModule = is_module<output_t, VIPRA::Modules::Type::OUTPUT> && can_write<output_t> &&
    can_write_timestep_values<output_t> && can_write_sim_values<output_t> && can_write_ped_values<output_t>;

class DummyOutput {
  // NOLINTBEGIN
 public:
  constexpr static VIPRA::Modules::Type _VIPRA_MODULE_TYPE_ = VIPRA::Modules::Type::OUTPUT;

  using output_data_t = void;

  template <typename params_t>
  void register_params(params_t&) {}

  void config(auto& /*unused*/) {}

  void write() {}
  void sim_value(const char* /*unused*/, auto&& /*unused*/) {}
  void timestep_value(const char* /*unused*/, VIPRA::timestep, auto&& /*unused*/) {}
  void ped_value(VIPRA::idx /*unused*/, const char* /*unused*/, auto&& /*unused*/) {}
  void ped_timestep_value(VIPRA::idx /*unused*/, VIPRA::timestep, const char* /*unused*/, auto&& /*unused*/) {
  }
  // NOLINTEND
};

CHECK_MODULE(OutputModule, DummyOutput);
}  // namespace VIPRA::Concepts
