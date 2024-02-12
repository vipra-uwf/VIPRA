#pragma once

#include <cstdio>
#include <fstream>
#include <utility>

#include "nlohmann/json.hpp"

#include "vipra/concepts/output.hpp"
#include "vipra/geometry/f3d.hpp"
#include "vipra/modules.hpp"
#include "vipra/types/idx.hpp"
#include "vipra/util/debug_do.hpp"

namespace VIPRA::Output::Trajectories {
class JSON {
 public:
  VIPRA_MODULE_TYPE(OUTPUT)

  template <typename params_t>
  void register_params(params_t& params) {
    params.register_param(_VIPRA_MODULE_TYPE_, "trajectories_json", "filename");
  }

  template <typename params_t>
  void config(params_t const& params) {
    _filename =
        params.template get_param<std::string>(Modules::Type::OUTPUT, "trajectories_json", "filename");
  }

  void write(std::filesystem::path const& outputDir) {
    std::filesystem::path filepath = outputDir / _filename;
    std::ofstream         file(filepath);
    if (!file.is_open()) {
      throw std::runtime_error("Could not open file for writing: " + filepath.string());
    }

    file << _json.dump();

    file.close();
  }

  template <typename data_t>
  void sim_value(char const* key, data_t&& value) {
    _json[key] = std::forward<data_t>(value);
  }

  template <typename data_t>
  void timestep_value(char const* key, VIPRA::idx timestep, data_t&& value) {
    _json["timesteps"][timestep][key] = std::forward<data_t>(value);
  }

  template <typename data_t>
  void ped_value(VIPRA::idx pedIdx, char const* key, data_t&& value) {
    _json["pedestrians"].at(pedIdx)[key] = std::forward<data_t>(value);
  }

  template <typename data_t>
  void ped_timestep_value(VIPRA::idx pedIdx, VIPRA::idx timestep, char const* key, data_t&& value) {
    _json["timesteps"][timestep]["pedestrians"][pedIdx][key] = std::forward<data_t>(value);
  }

 private:
  nlohmann::json _json;
  std::string    _filename;
};

template <>
inline void Output::Trajectories::JSON::ped_value(VIPRA::idx pedIdx, char const* key, VIPRA::f3d&& value) {
  _json["pedestrians"].at(pedIdx)[key] = nlohmann::json({value.x, value.y, value.z});
}
template <>
inline void Output::Trajectories::JSON::ped_value(VIPRA::idx pedIdx, char const* key, VIPRA::f3d& value) {
  _json["pedestrians"].at(pedIdx)[key] = nlohmann::json({value.x, value.y, value.z});
}
template <>
inline void Output::Trajectories::JSON::ped_value(VIPRA::idx pedIdx, char const* key,
                                                  const VIPRA::f3d& value) {
  _json["pedestrians"].at(pedIdx)[key] = nlohmann::json({value.x, value.y, value.z});
}

template <>
inline void Output::Trajectories::JSON::ped_timestep_value(VIPRA::idx pedIdx, VIPRA::idx timestep,
                                                           char const* key, VIPRA::f3d&& value) {
  _json["timesteps"][timestep]["pedestrians"][pedIdx][key] = nlohmann::json({value.x, value.y, value.z});
}

template <>
inline void Output::Trajectories::JSON::ped_timestep_value(VIPRA::idx pedIdx, VIPRA::idx timestep,
                                                           char const* key, const VIPRA::f3d& value) {
  _json["timesteps"][timestep]["pedestrians"][pedIdx][key] = nlohmann::json({value.x, value.y, value.z});
}

}  // namespace VIPRA::Output::Trajectories

CHECK_MODULE(OutputModule, VIPRA::Output::Trajectories::JSON)