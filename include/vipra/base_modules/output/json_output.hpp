#pragma once

#include <cstdio>
#include <fstream>
#include <utility>

#include "nlohmann/json.hpp"

#include "vipra/concepts/output.hpp"
#include "vipra/concepts/parameters.hpp"
#include "vipra/modules.hpp"
#include "vipra/types/f3d.hpp"
#include "vipra/types/idx.hpp"
#include "vipra/util/debug_do.hpp"

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
    Util::debug_do([&]() { std::printf("Writing JSON output to %s\n", _outputPath.string().c_str()); });

    std::ofstream file(_outputPath);
    if (!file.is_open()) {
      throw std::runtime_error("Could not open file for writing: " + _outputPath.string());
    }

    file << _json.dump(2);
  }

  template <typename data_t>
  void sim_value(const char* key, data_t&& value) {
    _json[key] = std::forward<data_t>(value);
  }

  template <typename data_t>
  void timestep_value(const char* key, VIPRA::idx timestep, data_t&& value) {
    _json["timesteps"][timestep][key] = std::forward<data_t>(value);
  }

  template <typename data_t>
  void ped_value(VIPRA::idx pedIdx, const char* key, data_t&& value) {
    _json["pedestrians"].at(pedIdx)[key] = std::forward<data_t>(value);
  }

  template <typename data_t>
  void ped_timestep_value(VIPRA::idx pedIdx, VIPRA::idx timestep, const char* key, data_t&& value) {
    _json["timesteps"][timestep]["pedestrians"][pedIdx][key] = std::forward<data_t>(value);
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

template <>
inline void Output::Trajectories::JSON::ped_timestep_value(VIPRA::idx pedIdx, VIPRA::idx timestep,
                                                           const char* key, VIPRA::f3d&& value) {
  _json["timesteps"][timestep]["pedestrians"][pedIdx][key] = nlohmann::json({value.x, value.y, value.z});
}

template <>
inline void Output::Trajectories::JSON::ped_timestep_value(VIPRA::idx pedIdx, VIPRA::idx timestep,
                                                           const char* key, const VIPRA::f3d& value) {
  _json["timesteps"][timestep]["pedestrians"][pedIdx][key] = nlohmann::json({value.x, value.y, value.z});
}

}  // namespace VIPRA::Output::Trajectories

CHECK_MODULE(OutputModule, VIPRA::Output::Trajectories::JSON)