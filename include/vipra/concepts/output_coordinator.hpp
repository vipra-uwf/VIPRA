#pragma once

#include "vipra/concepts/output.hpp"

namespace VIPRA::Concepts {

/**
 * @brief Checks that a type has a new_run method
 * 
 * @tparam output_t 
 */
template <typename output_t>
concept can_cycle_run = requires(output_t output) {
  {output.new_run(VIPRA::idx{})};
};

/**
 * @brief Checks that a type can write to an output
 * 
 * @tparam output_t 
 */
template <typename output_t>
concept can_coord_writes = requires(output_t output) {
  {output.write()};
};

/**
 * @brief Checks that a type is an output coordinator
 * 
 * @tparam output_t 
 */
template <typename output_t>
concept OutputCoordinator = BaseOutput<output_t> && can_cycle_run<output_t> && can_coord_writes<output_t>;

/**
 * @brief Dummy output coordinator for use in other concepts
 * 
 */
class DummyOutputCoordinator {
  // NOLINTBEGIN
 public:
  constexpr static VIPRA::Modules::Type _VIPRA_MODULE_TYPE_ = VIPRA::Modules::Type::OUTPUT;

  using output_data_t = void;

  void new_run(VIPRA::idx) {}

  template <typename params_t>
  void register_params(params_t&) {}

  void config(auto& /*unused*/) {}

  void write() {}
  void sim_value(char const* /*unused*/, auto&& /*unused*/) {}
  void timestep_value(char const* /*unused*/, VIPRA::timestep, auto&& /*unused*/) {}
  void ped_value(VIPRA::idx /*unused*/, char const* /*unused*/, auto&& /*unused*/) {}
  void ped_timestep_value(VIPRA::idx /*unused*/, VIPRA::timestep, char const* /*unused*/, auto&& /*unused*/) {
  }
  // NOLINTEND
};

CHECK_MODULE(OutputCoordinator, DummyOutputCoordinator);

}  // namespace VIPRA::Concepts