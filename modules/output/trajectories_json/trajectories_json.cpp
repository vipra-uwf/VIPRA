
#include "trajectories_json.hpp"
#include <fstream>
#include "vipra/macros/module.hpp"
#include "vipra/types/time.hpp"

namespace VIPRA::Output {

VIPRA_REGISTER_MODULE(TrajectoriesJson, Output)

void TrajectoriesJson::write(std::filesystem::path const& outputDir)
{
  std::filesystem::path filepath = outputDir / _filename;
  std::ofstream         file(filepath);

  file << _json.dump();
  file.close();
}

void TrajectoriesJson::timestep_update(VIPRA::timestep /*unused*/,
                                       VIPRA::delta_t /*unused*/,
                                       VIPRA::State const& state)
{
  _json["timesteps"].push_back(nlohmann::json::array());

  for ( auto const& position : state.positions ) {
    _json["timesteps"].back().push_back(
        nlohmann::json({position.x, position.y, position.z}));
  }
}
}  // namespace VIPRA::Output