#pragma once

#include <concepts>

#include "vipra/concepts/module.hpp"
#include "vipra/modules.hpp"

#include "vipra/types/f3d.hpp"
#include "vipra/types/idx.hpp"

namespace VIPRA::Concepts {
template <typename output_t>
concept OutputModule = Module<output_t, VIPRA::Modules::Type::OUTPUT> &&
    requires(output_t output, const char* key) {
  {output.write()};
  { output.sim_value(key, VIPRA::f3d{}) } -> std::same_as<void>;
  { output.timestep_value(key, VIPRA::f3d{}) } -> std::same_as<void>;
  { output.ped_value(VIPRA::idx{}, key, VIPRA::f3d{}) } -> std::same_as<void>;
  { output.ped_timestep_value(VIPRA::idx{}, key, VIPRA::f3d{}) } -> std::same_as<void>;
  { output.sim_value(key, std::string{}) } -> std::same_as<void>;
  { output.timestep_value(key, std::string{}) } -> std::same_as<void>;
  { output.ped_value(VIPRA::idx{}, key, std::string{}) } -> std::same_as<void>;
  { output.ped_timestep_value(VIPRA::idx{}, key, std::string{}) } -> std::same_as<void>;
  // { output.sim_value(key, const VIPRA::f3d& value) } -> std::same_as<void>;
  // { output.timestep_value(key, const VIPRA::f3d& value) } -> std::same_as<void>;
  // { output.ped_value(VIPRA::idx{}, key, const VIPRA::f3d& value) } -> std::same_as<void>;
  // { output.ped_timestep_value(VIPRA::idx{}, key, VIPRA::f3d & value) } -> std::same_as<void>;
  // { output.ped_timestep_value(VIPRA::idx{}, key, const VIPRA::f3d& value) } -> std::same_as<void>;
};

class DummyOutput {
  VIPRA_MODULE_TYPE(OUTPUT)
 public:
  using output_data_t = void;

  template <typename params_t>
  static void register_params() {}

  void setup(auto& /*unused*/) {}

  void write() {}
  void sim_value(const char* /*unused*/, auto&& /*unused*/) {}
  void timestep_value(const char* /*unused*/, auto&& /*unused*/) {}
  void ped_value(VIPRA::idx /*unused*/, const char* /*unused*/, auto&& /*unused*/) {}
  void ped_timestep_value(VIPRA::idx /*unused*/, const char* /*unused*/, auto&& /*unused*/) {}
};

CHECK_MODULE(OutputModule, DummyOutput);
}  // namespace VIPRA::Concepts
