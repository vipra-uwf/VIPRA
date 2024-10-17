
# Model Implementation Example

In this example we will walk through creating a [`Model`](../implementation/model.md) module.
This Model will simply have pedestrians walking in circles around their initial position.

1. Create Folder and Files
  We will create the following: 
  - `modules/model/ExampleModel`
  - `modules/model/ExampleModel/ExampleModel.hpp`
  - `modules/model/ExampleModel/CMakeLists.txt`

2. Create the model
```C++
#include <vipra.hpp>

// Create a new module, with it's C++ name and Module type (see Modules page for other types)
VIPRA_NEW_MODULE(ExampleModel, Model) {
 public:
  // Give the module a name and set its type
  VIPRA_MODULE_NAME("ExampleModel")
  VIPRA_MODULE_TYPE(Model)

  // Add in the parameter registration step, this is where we tell VIPRA what parameters the module needs
  VIPRA_REGISTER_PARAMS ( 
    VIPRA_PARAM("radius", _radius);
  )

  // Add in the initialization step, this is run right before the simulation starts
  VIPRA_MODEL_INIT_STEP {
    _initialPoints = pedset.all_coords();
  }

  // Add in the timestep, this is where the model updates the pedestrians state
  VIPRA_MODEL_TIMESTEP {
    for (size_t i = 0; i < pedset.num_pedestrians(); ++i) {
      // Update the pedestrian's position, to make them move in a circle
      state.positions[i] = VIPRA::f3d{_initialPoints[i][0] + _radius * std::cos(deltaT * timestep),
                                      _initialPoints[i][1] + _radius * std::sin(deltaT * timestep)};
    }
  }

 private:
  // Member variables to hold the parameters
  VIPRA::f_pnt  _radius;
  VIPRA::f3dVec _initialPoints;
};
```

3. Update `main.cpp` to use our new model

```C++
#include <vipra.hpp>

#include "modules/model/calm_model/calm_model.hpp"

auto main() -> int {

  // Create the simulation object, modules can be placed in any order so long as they are all there
  auto sim = VIPRA::simulation(
      Example::Model{},                       // Here, we use our new model
      VIPRA::Goals::AStar{},                  // Goals Module
      VIPRA::Pedestrians::Grid{},             // Pedestrian Set Module
      VIPRA::Map::QuadTree{}                  // Map Module
      VIPRA::Module::Output{                  // Output Coordinator
        VIPRA::Output::Trajectories::Json{}   // Output Module
      }
  );

  // Run the simulation, passing in parameters
  sim(
    VIPRA::Parameters{
      VIPRA::Input::Json{"maps/pedestrians/a320/a320_144_pedestrians.json"}, // Input Module for pedestrians
      VIPRA::Input::Json{"maps/obstacles/a320/a320_polygons.json"} // Input Module for obstalces
      VIPRA::Input::Json{"examples/module_params.json"} // Input Module for loading the parameters
    }
  );
}
```

4. Update the `module_params.json`

```JSON
{
  "simulation": {
    "main": {
      "max_timestep": 10000,
      "timestep_size": 0.005,
      "output_frequency": 100,
      "random_seed": 12345
    }
  },
  "model": {     // Since our module is a model we put the parameters under the "model" field
    "example_model": {  // We add in a field with our module name
      "radius": 0.2     // Here is our radius parameter
    }
  },
  "goals": {
    "astar": {
      "endGoalType": "exit",
      "gridSize": 0.1,
      "closestObstacle": 0.25,
      "goalRange": 0.05
    }
  },
  "pedestrians": {
    "grid": {
      "gridSize": 0.5
    }
  },
  "obstacles": {
    "quad_tree": {
      "minQuadSize": 0.05
    }
  },
  "output": {
    "coordinator": {
      "output_dir": "./output"
    },
    "trajectories_json": {
      "filename": "trajectories.json"
    }
  },
  "behavior_model": {
    "main": {
      "behaviors_dir": "./behaviors",
      "behaviors": []
    }
  }
}
```

5. Adding `CMakeLists.txt`

```CMake
vipra_add_all()
```

See the [Base Module Implementation](../implementation/module.md) to see more.

6. Compile Simulation

The simulation can be compiled by running
```
make
```

7. Run

Run the simulation with
```
./VIPRA
```

The output will be under `./output/trajectories.json`
---