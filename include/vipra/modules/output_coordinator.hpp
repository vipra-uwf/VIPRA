#pragma once

#include "vipra/modules/module.hpp"
#include "vipra/modules/output.hpp"

#include "vipra/types/idx.hpp"
#include "vipra/types/time.hpp"

namespace VIPRA::Modules {
/**
 * @brief Dummy output coordinator for use in other concepts
 * 
 */
template <typename module_t, typename... outputs_t>
class OutputCoordinator : public Module<OutputCoordinator<module_t, outputs_t...>>,
                          public Output<OutputCoordinator<module_t, outputs_t...>> {
 public:
  void new_run(VIPRA::idx runNumber) { this->self().new_run(runNumber); }

  auto write() {}
  void sim_value(char const*, auto&&) {}
  void timestep_value(char const*, VIPRA::timestep, auto&&) {}
  void ped_value(VIPRA::idx, char const*, auto&&) {}
  void ped_timestep_value(VIPRA::idx, VIPRA::timestep, char const*, auto&&) {}

 private:
};

}  // namespace VIPRA::Modules