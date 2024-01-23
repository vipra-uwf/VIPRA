#pragma once

#include "vipra/concepts/model.hpp"
#include "vipra/concepts/parameters.hpp"
#include "vipra/concepts/pedset.hpp"
#include "vipra/modules.hpp"

namespace CALM {
class Model {
  VIPRA_MODULE_TYPE(MODEL)

 public:
  void timestep(const auto& pedset, const auto& obsset) {}

  template <VIPRA::Concepts::ParamModule params_t>
  static void register_params() {
    params_t::register_param(MODULE_TYPE, "meanMass", VIPRA::Parameter{VIPRA::Parameter::Type::REQUIRED});
    params_t::register_param(MODULE_TYPE, "massStdDev", VIPRA::Parameter{VIPRA::Parameter::Type::REQUIRED});
    params_t::register_param(MODULE_TYPE, "meanReactionTime",
                             VIPRA::Parameter{VIPRA::Parameter::Type::REQUIRED});
    params_t::register_param(MODULE_TYPE, "reactionTimeStdDev",
                             VIPRA::Parameter{VIPRA::Parameter::Type::REQUIRED});
    params_t::register_param(MODULE_TYPE, "meanMaxSpeed", VIPRA::Parameter{VIPRA::Parameter::Type::REQUIRED});
    params_t::register_param(MODULE_TYPE, "maxSpeedStdDev",
                             VIPRA::Parameter{VIPRA::Parameter::Type::REQUIRED});
    params_t::register_param(MODULE_TYPE, "meanShoulderLen",
                             VIPRA::Parameter{VIPRA::Parameter::Type::REQUIRED});
    params_t::register_param(MODULE_TYPE, "shoulderLenStdDev",
                             VIPRA::Parameter{VIPRA::Parameter::Type::REQUIRED});
  }

 private:
};
}  // namespace CALM

CHECK_MODULE(ModelModule, CALM::Model)