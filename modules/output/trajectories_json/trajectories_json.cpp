
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
    auto const& timestep_pos = _trajectories[i];
    auto const& timestep_vel = (i < _velocities.size() ? _velocities[i]
                                                         : std::vector<VIPRA::f3d>{});
    file << '[';

    for ( size_t j = 0; j < timestep_pos.size(); ++j ) {
      auto const& position = timestep_pos[j];
      VIPRA::f3d const velocity = (j < timestep_vel.size() ? timestep_vel[j]
                                                        : VIPRA::f3d{0, 0, 0});

      // Output as an object with position and velocity arrays
      file << '{'
           << "\"position\":[" << position.x << ',' << position.y << ',' << position.z
           << "]" << ',' << "\"velocity\":[" << velocity.x << ',' << velocity.y << ',' << velocity.z
           << "]}";

      if ( j != timestep_pos.size() - 1 ) file << ',';
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
  // Inserts a new timestep at the end of the trajectories vector
  _trajectories.emplace_back(state.positions.size());

  // Inserts a new timestep at the end of the velocities vector
  _velocities.emplace_back(state.velocities.size());

  // For each pedestrian, add their position at this timestep
  for ( size_t i = 0; i < state.positions.size(); ++i ) {
    _trajectories.back()[i] = state.positions[i];
    if ( i < state.velocities.size() ) _velocities.back()[i] = state.velocities[i];
  }
} 
}  // namespace VIPRA::Output