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

  template <typename params_t>
  void set_params(params_t& params) {
    params.set_param(MODULE_TYPE, "meanMass", VIPRA::Parameter::Type::REQUIRED);
    params.set_param(MODULE_TYPE, "massStdDev", VIPRA::Parameter::Type::REQUIRED);
    params.set_param(MODULE_TYPE, "meanReactionTime", VIPRA::Parameter::Type::REQUIRED);
    params.set_param(MODULE_TYPE, "reactionTimeStdDev", VIPRA::Parameter::Type::REQUIRED);
    params.set_param(MODULE_TYPE, "meanMaxSpeed", VIPRA::Parameter::Type::REQUIRED);
    params.set_param(MODULE_TYPE, "maxSpeedStdDev", VIPRA::Parameter::Type::REQUIRED);
    params.set_param(MODULE_TYPE, "meanShoulderLen", VIPRA::Parameter::Type::REQUIRED);
    params.set_param(MODULE_TYPE, "shoulderLenStdDev", VIPRA::Parameter::Type::REQUIRED);
  }

 private:
};

// static_assert(VIPRA::Concepts::ModelModule<Model>);
}  // namespace CALM

CHECK_MODULE(ModelModule, CALM::Model)