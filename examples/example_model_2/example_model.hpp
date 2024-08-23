#include <vipra.hpp>

// Create a new module, with it's C++ name and Module type (see Modules page for other types)
NEW_VIPRA_MODULE(ExampleModel, Model)
{
 public:
  // Give the module a name and set its type
  VIPRA_MODULE_NAME("example_model")
  VIPRA_MODULE_TYPE(MODEL)

  // Add in the parameter registration step, this is where we tell VIPRA what parameters the module needs
  VIPRA_REGISTER_PARAMS(VIPRA_PARAM("radius", _radius))

  // Add in the initialization step, this is run right before the simulation starts
  VIPRA_MODEL_INIT_STEP { _initialPoints = pedset.all_coords(); }

  // Add in the timestep, this is where the model updates the pedestrians state
  VIPRA_MODEL_TIMESTEP
  {
    for ( size_t i = 0; i < pedset.num_pedestrians(); ++i ) {
      // Update the pedestrian's position, to make them move in a circle
      state.positions[i] = VIPRA::f3d{_initialPoints[i][0] + _radius * std::cos(deltaT * timestep),
                                      _initialPoints[i][1] + _radius * std::sin(deltaT * timestep)};
    }
  }

 private:
  VIPRA::f_pnt  _radius;
  VIPRA::f3dVec _initialPoints;
};