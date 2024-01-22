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
  { output.write() } -> std::same_as<void>;
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
}  // namespace VIPRA::Concepts