#pragma once

#include <iostream>
#include "vipra/concepts/model.hpp"
#include "vipra/concepts/parameters.hpp"
#include "vipra/concepts/pedset.hpp"
#include "vipra/modules.hpp"
#include "vipra/types/float.hpp"
#include "vipra/util/debug_do.hpp"

namespace CALM {
class Model {
  VIPRA_MODULE_TYPE(MODEL)
  struct Data {
    std::vector<VIPRA::f_pnt> masses;
    std::vector<VIPRA::f_pnt> reactionTimes;
    std::vector<VIPRA::f_pnt> maxSpeeds;
    std::vector<VIPRA::f_pnt> shoulderLens;

    void resize(size_t size) {
      masses.resize(size);
      reactionTimes.resize(size);
      maxSpeeds.resize(size);
      shoulderLens.resize(size);
    }
  };

 public:
  template <VIPRA::Concepts::PedsetModule pedset_t>
  void initialize(const pedset_t& pedset) {
    _peds.resize(pedset.num_pedestrians());
    _state.initialize(pedset.num_pedestrians());
  }

  template <VIPRA::Concepts::PedsetModule peds_t, VIPRA::Concepts::MapModule map_t>
  auto timestep(const peds_t& pedset, const map_t& obsset) -> const VIPRA::State& {
    // TODO(rolland): implement
    VIPRA::Util::debug_do([]() { std::cout << "Model timestep" << std::endl; });

    return _state;
  }

  template <VIPRA::Concepts::ParamModule params_t>
  static void register_params() {
    params_t::register_param(MODULE_TYPE, "meanMass", VIPRA::ParameterType::REQUIRED);
    params_t::register_param(MODULE_TYPE, "massStdDev", VIPRA::ParameterType::REQUIRED);
    params_t::register_param(MODULE_TYPE, "meanReactionTime", VIPRA::ParameterType::REQUIRED);
    params_t::register_param(MODULE_TYPE, "reactionTimeStdDev", VIPRA::ParameterType::REQUIRED);
    params_t::register_param(MODULE_TYPE, "meanMaxSpeed", VIPRA::ParameterType::REQUIRED);
    params_t::register_param(MODULE_TYPE, "maxSpeedStdDev", VIPRA::ParameterType::REQUIRED);
    params_t::register_param(MODULE_TYPE, "meanShoulderLen", VIPRA::ParameterType::REQUIRED);
    params_t::register_param(MODULE_TYPE, "shoulderLenStdDev", VIPRA::ParameterType::REQUIRED);
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
  Data         _peds;
  VIPRA::State _state;
};
}  // namespace CALM

CHECK_MODULE(ModelModule, CALM::Model)