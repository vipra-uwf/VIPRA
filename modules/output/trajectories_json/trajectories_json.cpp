
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

  file << "{\"trajectories\":[";
  for ( size_t i = 0; i < _trajectories.size(); ++i ) {
    auto const& timestep = _trajectories[i];
    file << '[';

    for ( size_t j = 0; j < timestep.size(); ++j ) {
      auto const& position = timestep[j];
      file << '[' << position.x << ',' << position.y << ',' << position.z << ']';
      if ( j != timestep.size() - 1 ) file << ',';
    }

    file << ']';
    if ( i != _trajectories.size() - 1 ) file << ',';
  }
  file << "]}";
  file.close();
}

void TrajectoriesJson::timestep_update(VIPRA::timestep /*unused*/,
                                       VIPRA::delta_t /*unused*/,
                                       VIPRA::State const& state)
{
  _trajectories.emplace_back(state.positions.size());

  for ( size_t i = 0; i < state.positions.size(); ++i ) {
    _trajectories.back()[i] = state.positions[i];
  }
}
}  // namespace VIPRA::Output