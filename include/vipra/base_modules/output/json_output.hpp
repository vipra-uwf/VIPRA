#pragma once

#include <cstdio>
#include <fstream>

#include "nlohmann/json.hpp"

#include "vipra/concepts/output.hpp"
#include "vipra/concepts/parameters.hpp"
#include "vipra/modules.hpp"
#include "vipra/types/f3d.hpp"
#include "vipra/types/idx.hpp"

namespace VIPRA::Output::Trajectories {
class JSON {
  VIPRA_MODULE_TYPE(OUTPUT)

 public:
  template <Concepts::ParamModule params_t>
  static void register_params() {
    params_t::register_param(MODULE_TYPE, "filepath", ParameterType::REQUIRED);
  }

  template <Concepts::ParamModule params_t>
  void setup(params_t& params) {
    _outputPath = params.template get_param<std::string>(Modules::Type::OUTPUT, "filepath");
  }

  void write() {
    std::ofstream file(_outputPath);
    if (!file.is_open()) {
      throw std::runtime_error("Could not open file for writing: " + _outputPath.string());
    }

    file << _json.dump(2);
  }

  void current_state(const VIPRA::State& /*unused*/) {}

  void sim_value(const char* key, auto&& value) { _json[key] = value; }
  void timestep_value(const char* key, auto&& value) {
    _json["timesteps"].push_back(nlohmann::json({key, value}));
  }
  void ped_value(VIPRA::idx pedIdx, const char* key, auto&& value) {
    _json["pedestrians"].at(pedIdx)[key] = value;
  }
  void ped_timestep_value(VIPRA::idx pedIdx, const char* key, auto&& value) {
    _json["pedestrians"].at(pedIdx)["timesteps"].push_back(nlohmann::json({key, value}));
  }

 private:
  nlohmann::json        _json;
  std::filesystem::path _outputPath;
};

template <>
inline void Output::Trajectories::JSON::ped_value(VIPRA::idx pedIdx, const char* key, VIPRA::f3d&& value) {
  _json["pedestrians"].at(pedIdx)[key] = nlohmann::json({value.x, value.y, value.z});
}
template <>
inline void Output::Trajectories::JSON::ped_value(VIPRA::idx pedIdx, const char* key, VIPRA::f3d& value) {
  _json["pedestrians"].at(pedIdx)[key] = nlohmann::json({value.x, value.y, value.z});
}
template <>
inline void Output::Trajectories::JSON::ped_value(VIPRA::idx pedIdx, const char* key,
                                                  const VIPRA::f3d& value) {
  _json["pedestrians"].at(pedIdx)[key] = nlohmann::json({value.x, value.y, value.z});
}
}  // namespace VIPRA::Output::Trajectories

CHECK_MODULE(OutputModule, VIPRA::Output::Trajectories::JSON)