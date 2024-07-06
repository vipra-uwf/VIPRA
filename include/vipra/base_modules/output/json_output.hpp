#pragma once

#include <cstdio>
#include <fstream>
#include <utility>

#include <nlohmann/json.hpp>

#include "vipra/modules/output.hpp"

#include "vipra/macros/module.hpp"
#include "vipra/macros/parameters.hpp"

#include "vipra/geometry/f3d.hpp"

namespace VIPRA::Output::Trajectories {
/**
 * @brief JSON output module for writing trajectories to a JSON file
 * 
 */
class JSON : public VIPRA::Modules::Output<JSON> {
 public:
  VIPRA_MODULE_NAME("trajectories_json")
  VIPRA_MODULE_TYPE(OUTPUT)

  VIPRA_REGISTER_STEP { VIPRA_REGISTER_PARAM("filename", _filename); }

  [[nodiscard]] auto to_string() -> std::string { return _json.dump(); }

  template <typename data_t>
  void set_sim_value(char const* key, data_t&& value) {
    _json[key] = std::forward<data_t>(value);
  }

  template <typename data_t>
  void set_timestep_value(char const* key, VIPRA::idx timestep, data_t&& value) {
    _json["timesteps"][timestep][key] = std::forward<data_t>(value);
  }

  template <typename data_t>
  void set_ped_value(VIPRA::idx pedIdx, char const* key, data_t&& value) {
    _json["pedestrians"].at(pedIdx)[key] = std::forward<data_t>(value);
  }

  template <typename data_t>
  void set_ped_timestep_value(VIPRA::idx pedIdx, VIPRA::idx timestep, char const* key, data_t&& value) {
    _json["timesteps"][timestep]["pedestrians"][pedIdx][key] = std::forward<data_t>(value);
  }

 private:
  nlohmann::json _json;
  std::string    _filename;
};

template <>
inline void Output::Trajectories::JSON::set_ped_value(VIPRA::idx pedIdx, char const* key,
                                                      VIPRA::f3d&& value) {
  _json["pedestrians"].at(pedIdx)[key] = nlohmann::json({value.x, value.y, value.z});
}
template <>
inline void Output::Trajectories::JSON::set_ped_value(VIPRA::idx pedIdx, char const* key, VIPRA::f3d& value) {
  _json["pedestrians"].at(pedIdx)[key] = nlohmann::json({value.x, value.y, value.z});
}
template <>
inline void Output::Trajectories::JSON::set_ped_value(VIPRA::idx pedIdx, char const* key,
                                                      const VIPRA::f3d& value) {
  _json["pedestrians"].at(pedIdx)[key] = nlohmann::json({value.x, value.y, value.z});
}

template <>
inline void Output::Trajectories::JSON::set_ped_timestep_value(VIPRA::idx pedIdx, VIPRA::idx timestep,
                                                               char const* key, VIPRA::f3d&& value) {
  _json["timesteps"][timestep]["pedestrians"][pedIdx][key] = nlohmann::json({value.x, value.y, value.z});
}

template <>
inline void Output::Trajectories::JSON::set_ped_timestep_value(VIPRA::idx pedIdx, VIPRA::idx timestep,
                                                               char const* key, const VIPRA::f3d& value) {
  _json["timesteps"][timestep]["pedestrians"][pedIdx][key] = nlohmann::json({value.x, value.y, value.z});
}

}  // namespace VIPRA::Output::Trajectories
