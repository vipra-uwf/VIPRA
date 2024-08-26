
#include "example_model.hpp"

void ExampleModel::views_timestep(VIPRA::Views::Pedestrians pedset, VIPRA::State& state,
                                  VIPRA::delta_t deltaT, VIPRA::timestep timestep)
{
  for ( size_t i = 0; i < pedset.num_pedestrians(); ++i ) {
    // Update the pedestrian's position, to make them move in a circle
    state.positions[i] = VIPRA::f3d{_initialPoints[i][0] + _radius * std::cos(deltaT * timestep),
                                    _initialPoints[i][1] + _radius * std::sin(deltaT * timestep)};
  }
}