---
title: Example Simulation
layout: template
filename: simluation.md
--- 

# Example Simulation

Below is an example main that uses some of the module implementations that are shipped with VIPRA.

```C++
#include <vipra.hpp>

#include "modules/model/calm_model/calm_model.hpp"

auto main() -> int {
  // Create the simulation, modules can be placed in any order so long as all are provided/valid
  auto sim = VIPRA::simulation(
      CALM::Model{},                          // Pedestrian Dynamics Model
      VIPRA::Goals::AStar{},                  // Goals Module
      VIPRA::Pedestrians::Grid{},             // Pedestrian Set Module
      VIPRA::Obstacles::QuadTree{}            // Obstacle Set Module
      VIPRA::Module::Output{                  // Output Coordinator
        VIPRA::Output::Trajectories::JSON{}   // Output Module
      }
  );

  // Run the simulation, passing in parameters
  sim(
    VIPRA::Parameters{
      VIPRA::Input::JSON{"maps/pedestrians/a320/a320_144_pedestrians.json"}, // Input Module for pedestrians
      VIPRA::Input::JSON{"maps/obstacles/a320/a320_polygons.json"} // Input Module for obstalces
      VIPRA::Input::JSON{"module_params.json"} // Input Module for loading the parameters
    }
  );
}
```

Follow the [example for implementing a different pedestrian dynamics model](model_example.md) to see how to change the simulation.