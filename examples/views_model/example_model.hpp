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
  // Using VIPRA::Views the method can be defined in the .cpp file, speeding up compilation times
  // This reduces performance and it is recommended to not use them for finalized modules
  VIPRA_MODEL_TIMESTEP { views_timestep(pedset, state, deltaT, timestep); }

 private:
  VIPRA::f_pnt  _radius;
  VIPRA::f3dVec _initialPoints;

  // Using VIPRA::Views the method can be defined in the .cpp file, speeding up compilation times
  // This reduces performance and it is recommended to not use them for finalized modules
  void views_timestep(VIPRA::Views::Pedestrians pedset, VIPRA::State & state, VIPRA::delta_t deltaT,
                      VIPRA::timestep timestep);
};