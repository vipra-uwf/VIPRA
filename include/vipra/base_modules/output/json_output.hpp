#pragma once

#include <cstdio>
#include <fstream>

#include "nlohmann/json.hpp"

#include "vipra/concepts/parameters.hpp"
#include "vipra/modules.hpp"
#include "vipra/types/f3d.hpp"
#include "vipra/types/idx.hpp"

namespace VIPRA::Output {
class JSON {
  VIPRA_MODULE_TYPE(OUTPUT)

 public:
  explicit JSON(const char* filename) : _outputPath(filename) {}

  template <typename params_t>
  void set_params(params_t& params) {
    params.set_param(MODULE_TYPE, "filename", VIPRA::Parameter{Parameter::Type::REQUIRED});
  }

  void write() {
    std::ofstream file(_outputPath);
    if (!file.is_open()) {
      throw std::runtime_error("Could not open file for writing: " + _outputPath.string());
    }

    file << _json.dump(2);
  }
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
inline void Output::JSON::ped_value(VIPRA::idx pedIdx, const char* key, VIPRA::f3d&& value) {
  _json["pedestrians"].at(pedIdx)[key] = nlohmann::json({value.x, value.y, value.z});
}
template <>
inline void Output::JSON::ped_value(VIPRA::idx pedIdx, const char* key, VIPRA::f3d& value) {
  _json["pedestrians"].at(pedIdx)[key] = nlohmann::json({value.x, value.y, value.z});
}
template <>
inline void Output::JSON::ped_value(VIPRA::idx pedIdx, const char* key, const VIPRA::f3d& value) {
  _json["pedestrians"].at(pedIdx)[key] = nlohmann::json({value.x, value.y, value.z});
}
}  // namespace VIPRA::Output