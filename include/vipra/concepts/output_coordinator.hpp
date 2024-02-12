#pragma once

#include "vipra/concepts/output.hpp"

namespace VIPRA::Concepts {

template <typename output_t>
concept can_cycle_run = requires(output_t output) {
  {output.new_run(VIPRA::idx{})};
};

template <typename output_t>
concept can_coord_writes = requires(output_t output) {
  {output.write()};
};

template <typename output_t>
concept OutputCoordinator = BaseOutput<output_t> && can_cycle_run<output_t> && can_coord_writes<output_t>;

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
  void sim_value(const char* /*unused*/, auto&& /*unused*/) {}
  void timestep_value(const char* /*unused*/, VIPRA::timestep, auto&& /*unused*/) {}
  void ped_value(VIPRA::idx /*unused*/, const char* /*unused*/, auto&& /*unused*/) {}
  void ped_timestep_value(VIPRA::idx /*unused*/, VIPRA::timestep, const char* /*unused*/, auto&& /*unused*/) {
  }
  // NOLINTEND
};

CHECK_MODULE(OutputCoordinator, DummyOutputCoordinator);

}  // namespace VIPRA::Concepts