#pragma once

#include <cstdio>

#include <nlohmann/json.hpp>

#include "vipra/macros/output.hpp"
#include "vipra/modules/output.hpp"

#include "vipra/macros/module.hpp"
#include "vipra/macros/parameters.hpp"

namespace VIPRA::Output {
/**
 * @brief JSON output module for writing trajectories to a JSON file
 * 
 */
class TrajectoriesJson : public VIPRA::Modules::Module<TrajectoriesJson>,
                         public VIPRA::Modules::Output {
 public:
  VIPRA_MODULE_NAME("TrajectoriesJson")
  VIPRA_MODULE_TYPE(Output)

  VIPRA_REGISTER_PARAMS(VIPRA_PARAM("filename", _filename))

  VIPRA_OUTPUT_RESET override { _json.clear(); }

  void write(std::filesystem::path const& outputDir) override;
  void timestep_update(VIPRA::timestep /*unused*/, VIPRA::delta_t /*unused*/,
                       VIPRA::State const& state) override;

 private:
  nlohmann::json _json;
  std::string    _filename;
};
}  // namespace VIPRA::Output
