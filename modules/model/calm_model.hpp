#pragma once

#include "vipra/concepts/model.hpp"
#include "vipra/concepts/parameters.hpp"
#include "vipra/concepts/pedset.hpp"
#include "vipra/modules.hpp"
#include "vipra/types/float.hpp"

namespace CALM {
class Model {
  VIPRA_MODULE_TYPE(MODEL)
  struct Data {
    std::vector<VIPRA::f_pnt> masses;
    std::vector<VIPRA::f_pnt> reactionTimes;
    std::vector<VIPRA::f_pnt> maxSpeeds;
    std::vector<VIPRA::f_pnt> shoulderLens;
  };

 public:
  template <VIPRA::Concepts::PedsetModule pedset_t>
  void initialize(const pedset_t& pedset) {
    // Data data;
    // data.masses = pedset.get_vector<VIPRA::f_pnt>("mass");
    // data.reactionTimes = pedset.get_vector<VIPRA::f_pnt>("reactionTime");
    // data.maxSpeeds = pedset.get_vector<VIPRA::f_pnt>("maxSpeed");
    // data.shoulderLens = pedset.get_vector<VIPRA::f_pnt>("shoulderLen");
  }

  template <VIPRA::Concepts::PedsetModule peds_t, VIPRA::Concepts::MapModule map_t>
  void timestep(const peds_t& pedset, const map_t& obsset) {
    // TODO(rolland): implement
  }

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

  void setup(const auto& params) {
    auto meanMass = params.template get_param<VIPRA::f_pnt>(MODULE_TYPE, "meanMass");
    auto massStdDev = params.template get_param<VIPRA::f_pnt>(MODULE_TYPE, "massStdDev");
    auto meanReactionTime = params.template get_param<VIPRA::f_pnt>(MODULE_TYPE, "meanReactionTime");
    auto reactionTimeStdDev = params.template get_param<VIPRA::f_pnt>(MODULE_TYPE, "reactionTimeStdDev");
    auto meanMaxSpeed = params.template get_param<VIPRA::f_pnt>(MODULE_TYPE, "meanMaxSpeed");
    auto maxSpeedStdDev = params.template get_param<VIPRA::f_pnt>(MODULE_TYPE, "maxSpeedStdDev");
    auto meanShoulderLen = params.template get_param<VIPRA::f_pnt>(MODULE_TYPE, "meanShoulderLen");
    auto shoulderLenStdDev = params.template get_param<VIPRA::f_pnt>(MODULE_TYPE, "shoulderLenStdDev");
  }

 private:
};
}  // namespace CALM

CHECK_MODULE(ModelModule, CALM::Model)