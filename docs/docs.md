\tableofcontents

# Usage

VIPRA is a template framework this means it requires the user to define their own main and handle any command line arguments.

A basic skeleton will be provided in the future for ease of use.

There is an [Example Simulation](#example-simulation) below.

## Simulation Construction

Simulations are created by choosing which implementations to use for the several different [Module Types](#module-types).

A utility function for creating simulations is provided that allows the modules to be passed in in any order:
```C++
template <typename... module_ts>
auto VIPRA::simulation(module_ts&&...) -> VIPRA::SimType;
```

### Required Modules

Each simulation requires the following modules:
- `Model`
- `Goals`
- `Pedestrian Set`
- `Map`
  - `Obstacle Set`
  - `Polygon Input`
- `Output Coordinator`
  - `Output` (Can have any number of Output modules)

`Maps` and `Output Coordinators` are special in that they require modules themselves (denoted by the additional indent)

---

## Example Simulation

Below is an example main that uses some of the module implementations that are shipped with VIPRA.

```C++
#include <vipra.hpp>

#include "modules/model/calm_model/calm_model.hpp"

auto main() -> int {
  // Create the simulation, modules can be placed in any order so long as all are provided/valid
  auto sim = VIPRA::simulation(
      CALM::Model{},                          // Pedestrian Dynamics Model
      VIPRA::Goals::AStar{},                  // Goals Module
      VIPRA::Pedestrians::Grid{               // Pedestrian Set Module
          VIPRA::Input::JSON{"maps/pedestrians/a320/a320_144_pedestrians.json"}, // Input Module
      },
      VIPRA::Module::Output{                  // Output Coordinator
        VIPRA::Output::Trajectories::JSON{}   // Output Module
      },
      VIPRA::Module::Map{                     // Map Coordinator
        VIPRA::Input::JSON{"maps/obstacles/a320/a320_polygons.json"}, // Input Module (Polygon Input)
        VIPRA::Obstacles::QuadTree{}          // Obstacle Set Module
      }
  );

  // Run the simulation, passing in parameters
  sim(
    VIPRA::Parameters{
      VIPRA::Input::JSON{"module_params.json"}
    }
  );
}
```

## Example Parameter Sweep

This example uses the parameter sweep engine.

It takes in the total number of simulations to run as a command line argument.

```C++
#include <vipra.hpp>

#include "modules/model/calm_model/calm_model.hpp"

auto main(int argc, char** argv) -> int {
  VIPRA::ParameterSweep::initialize(argc, argv);  // Initialize the parameter sweep engine

  if (argc != 2) {      // Read in the command line arguments for the number of simulations to run
    std::cerr << "Usage: " << argv[0] << " <simCount>" << std::endl;
    return 1;
  }
  size_t simCount = std::stoul(argv[1]);

  auto sim = VIPRA::simulation(     // Create the simulation
      CALM::Model{}, 
      VIPRA::Goals::AStar{},
      VIPRA::Pedestrians::Grid{
          VIPRA::Input::JSON{"maps/pedestrians/a320/a320_144_pedestrians.json"},
      },
      VIPRA::Module::Output{VIPRA::Output::Trajectories::JSON{}},
      VIPRA::Module::Map{
        VIPRA::Input::JSON{"maps/obstacles/a320/a320_polygons.json"},
        VIPRA::Obstacles::QuadTree{}
      });

  VIPRA::ParameterSweep::run(    // Run a parameter sweep
      sim,                       // Simulation to run
      VIPRA::Parameters{VIPRA::Input::JSON{"module_params.json"}}, // Module Parameters
      simCount,                  // Number of simulations
      []() { std::cout << "Simulation complete on: " << VIPRA::ParameterSweep::get_rank() << std::endl; } // Optional: callback for each simulation run
  );
}
```

## Module Parameters

Parameters are loaded into each module before they are initizalized. The exact steps of a simulation run are shown in [`Simluation Steps`](#simulation-steps).

Parameters can use any [`Parameter Input`](#parameter-input-implementation) module, the [`JSON`](#json-input) input module provided works for this.

The format for JSON parameters is:
```JSON
{
  "module type": {
    "module name": {
      "parameter name": "parameter value"
    }
  },
  ...
}
```

Example of a [`Goals`](#goals-implementation) module parameters:
```JSON
{
  ...
  "goals": {                  // Module Type
    "astar": {                // Module Name
      "endGoalType": "exit",  // Parameter values
      "gridSize": 0.1,
      "closestObstacle": 0.25,
      "goalRange": 0.05
    }
  },
  ...
}
```
---

## Included Modules

There are several modules included with VIPRA.

**Model**:
- [`Calm Model`](#calm-model)

**Goals**:
- [`AStar`](#astar-goals)

**Obstacle Set**:
- [`QuadTree`](#quadtree-obstalce-set)

**Pedestrian Set**:
- [`Grid`](#grid-pedestrian-set)

**Input**:
- [`JSON`](#json-input)

**Output**:
- [`JSON`](#json-output)

---

### Calm Model

This `Model` module implements the [Calm Model](https://journals.plos.org/plosone/article?id=10.1371/journal.pone.0229690)

#### Use:
```C++
#include "modules/model/calm_model/calm_model.hpp"

VIPRA::simulation(
  ...
  CALM::Model{},
  ...
);
```

#### Parameters:
- `meanMass` : Mean mass for population
- `massStdDev` : Standard Deviation for mass
- `meanReactionTime` : Mean reaction time for population
- `reactionTimeStdDev` : Standard Deviation for reaction time
- `meanMaxSpeed` : Mean max speed for population
- `maxSpeedStdDev` : Standard deviation for max speed
- `meanShoulderLen` : Mean shoulder length for population
- `shoulderLenStdDev` : Standard deviation for shoulder length

### AStar Goals

This `Goals` module uses the [A* algorithm](#astar-algorithm) to find paths for each pedestrian.

#### Use:
```C++
#include "vipra.hpp"

VIPRA::simulation(
  ...
  VIPRA::Goals::AStar{},
  ...
);
```

#### Parameters:
- `endGoalType` : Name of object in map each pedestrian tries to reach (e.g. "exit")
- `goalRange` : Range, in meters, before a goal is counted as "reached" (e.g. 0.05)
- `gridSize` : Length of each side, in meters, of each grid in the pathing graph (e.g. 0.1)
- `closestObstacle` : Closest a grid center can be to an obstacle before being considered non-traversable

### QuadTree Obstacle Set 

This `Obstacle Set` module uses the [Quad Tree](#quadtree-datastructure) to hold the map geometry.

#### Use:
```C++
#include "vipra.hpp"

VIPRA::simulation(
  ...
  VIPRA::Module::Map{
    VIPRA::Input::JSON{"filepath"} // Map requires an Input module
    VIPRA::Obstacles::QuadTree{} // Obstacle sets go inside the Map module
  }
  ...
)
```

#### Parameters:

- `minQuadSize` : Size at which quads stop being subdivided

### Grid Pedestrian Set

This `Pedestrian Set` module holds pedestrians in a grid of cells, allowing for more efficient lookup of nearest neighbors

#### Use:
```C++
#include "vipra.hpp"

VIPRA::simulation(
  ...
  VIPRA::Pedestrians::Grid{},
  ...
);
```

#### Parameters:

- `gridSize`: Size of each grid cell

### JSON Input

This `Input` module loads JSON data, using [nlohmann JSON](https://github.com/nlohmann/json).
- Qualifies as a:
  - Polygon Input
  - Parameter Input

#### Use:

```C++
#include "vipra.hpp"

VIPRA::simulation(
  ...
  VIPRA::Module::Map {
    VIPRA::Input::JSON{"filepath"} // Input modules are usually used in other modules
    ...
  }
);

sim(VIPRA::Parameters{
  VIPRA::Input::JSON{"filepath"} // Or for loading parameters
});
```

#### Parameters:

- NONE

### JSON Output

This `Output` module creates a JSON file containing the positions of each pedestrian for each timestep that matches the output frequency in the simulation module parameters.

#### Use:

```C++
VIPRA::simulation(
  ...
  VIPRA::Module::Output{
    VIPRA::Output::Trajectories::JSON{}
  },
  ...
)
```

#### Parameters:

- `filename`: Name of the output file

---

# Common Errors

<!-- TODO -->

---

# Development

# Overview

<!-- TODO -->

## Simulation

### Simulation Steps

These are the general steps taken when a simulation is run.

#### 1. VIPRA::simulation

First a Simulation is constructed using the `VIPRA::simulation` utility method.

#### 2. Run Sim

The simulation is started using either `operator()` or `run_sim()`.

These both take a [`Parameter Module`](#parameter-module) as their argument.

#### 3. Parameters Loaded

The [`Parameter Module`](#parameter-module) calls `load()` on it's [`Parameter Input`](#parameter-input-implementation) module.

#### 4. Parameters Registered

Each Module has its `register_params()` method called.

This is used to tell the simulation which parameters are required by each module.

#### 5. Modules Configured

Each Module has its `config()` method called.

The loaded parameters are passed in to each module.


#### 6. Modules Initialized

The [`Map`](#map-module), [`Goals`](#goals-implementation), [`Model`](#model-implementation), and [`Behavior Model`](#behaviors) have their `initialize()` methods called.

This allows each module to setup any necessary pre-simulation work.

#### 7. Simulation Run

The following is run until the [`Goals`](#goals-implementation) module simulation goal is met, or until the simulation `max_timestep` is reached.

```
model.timestep();
behaviors.timestep();
pedestrian_set.update();
goals.update();
```

#### 8. Output

Finally, each of the [`Output`](#output-implementation) modules `write()` method is called.

## Module Types

**Model**

[`Model`](#model-implementation) modules are the pedestrian dynamics models that govern how the pedestrians will move during a simulation

**Goals**

[`Goals`](#goals-implementation) modules handle pathfinding for the simulation

**Obstacle Set**

[`Obstacle Set`](#obstacle-set-implementation) modules hold the geometry of the environment and handle map/pedestrian collision detection

**Pedestrian Set**

[`Pedestrian Set`](#pedestrian-set-implementation) modules hold the pedestrian positions/velocities and handle queries to pedestrians (ex. nearest neighbor)

**Input**

[`Input`](#input-implementation) modules handle loading input for the simulation. There are several specialized input types:
- [`Serializable Input`](#serializable-input-implementation) modules are inputs that allow for serialization of their values
- [`Parameter Input`](#parameter-input-implementation) modules are able to load parameters
- [`Polygon Input`](#polygon-input-implementation) modules are able to load polygons for use in obstacle sets

**Output**

[`Output`](#output-implementation) modules handle producing output from the simulation

**Map**

The [`Map`](#map) module is a coordinator, it accepts one [`Obstacle Set`](#obstacle-set-implementation) and one [`Polygon Input`](#polygon-input-implementation).

**Output Coordinator**

The [`Output Coordinator`](#output-coordinator) module handles coordinating any number of [`Output`](#output-implementation) modules.

## Parameter Sweep

<!-- TODO: Add in parameter sweep stuff -->

## Randomization

<!-- TODO: add in about how randomization should be handled -->

## VIPRA Types

<!-- TODO: add in about VIPRA specific types -->

# Module Implementation

> !!! Important Notes:
> - These modules are implemented using templates; this means that any method that accepts a module as a parameter is required to be defined in a header file
> - Every module has helper macros for their required methods, see [Macros](#macros)
> - For these Interfaces, assume `using namespace VIPRA::Concepts`

# Base Module Implementation

Each module is required to implement the following:

### Interface

```C++
void register_params(Params auto&);
void config(Params auto const&);
```

Every Module also requires a name and type tag:

```C++
VIPRA_MODULE_NAME("**name**")
VIPRA_MODULE_TYPE(**type**)
```

- Replace `**name**` with the name for the module; this will be used to pull the correct parameters from input.
- Replace `**type**` with the type of module the class is; available modules are below:

1. `MODEL` - [Model](#model)
2. `GOALS` - [Goals](#goals)
3. `PEDESTRIANS` - [Pedestrian Set](#pedestrian-set)
4. `OBSTACLES` - [Obstacle Set](#obstacle-set)
5. `INPUT` - [Input](#input)
6. `OUTPUT` - [Output](#output)

# Model Implementation

`Model` modules are the pedestrian dynamics models that govern how the pedestrians will move during a simulation

### Interface

Along with the [Base Module](#base-module-implementation) interface, Model modules are required to implement the following methods.

Helper macros can be found in [Model Macros](#model-macros)

```C++
void initialize(PedSet auto const&, Map auto const&, Goals auto const&, Output auto&);

void timestep(PedSet auto const&, Map auto const&, Goals auto const&, Output auto&, VIPRA::State&, VIPRA::delta_t, VIPRA::timestep);
```

# Goals Implementation

`Goals` modules handle pathfinding for the simulation.

### Interface

Along with the [Base Module](#base-module-implementation) interface, Goals modules are required to implement the following methods.

Helper macros can be found in [Goals Macros](#goals-macros)

```C++
void initialize(PedSet auto const&, Map auto const&);
void update(PedSet auto const&, Map auto const&, VIPRA::delta_t);
void change_end_goal(VIPRA::idx, VIPRA::f3d);

auto current_goals() -> std::vector<VIPRA::f3d> const&;
auto end_goals() -> std::vector<VIPRA::f3d> const&;
auto current_goal(VIPRA::idx) -> VIPRA::f3d const&;
auto end_goal(VIPRA::idx) -> VIPRA::f3d const&;
auto is_goal_met(VIPRA::idx) -> bool;
auto is_sim_goal_met() -> bool;
auto time_since_last_goal(VIPRA::idx) -> VIPRA::f_pnt;
```

# Obstacle Set Implementation

Obstacle Set modules hold the geometry of the environment and handle map/pedestrian collision detection.

### Interface

Along with the [Base Module](#base-module-implementation) interface, Obstacle Set modules are required to implement the following methods.

Helper macros can be found in [Obstacle Set Macros](#obstacle-set-macros)

```C++
void initialize(std::vector<VIPRA::Geometry::Polygon> const&, std::vector<std::string> const&, std::map<std::string, std::vector<VIPRA::f3d>> const&);

auto get_object_types() -> std::vector<std::string> const&;
auto get_objects(std::string const&) -> std::vector<VIPRA::f3d> const&;

auto collision(VIPRA:f3d) -> bool;
auto collision(VIPRA::Geometry::Circle) -> bool;
auto ray_hit(VIPRA::f3d, VIPRA::f3d) -> VIPRA::f_pnt;

auto get_dimensions() -> VIPRA::f3d;
```

# Pedestrian Set Implementation

### Interface

Along with the [Base Module](#base-module-implementation) interface, Obstacle Set modules are required to implement the following methods.

Helper macros can be found in [Pedestrian Set Macros](#pedestrian-set-macros)

```C++
auto num_pedestrians() -> VIPRA::size;

auto ped_coords(VIPRA::idx) -> VIPRA::f3d;
auto all_coords() -> std::vector<VIPRA::f3d> const&;
auto ped_velocity(VIPRA::idx) -> VIPRA::f3d;
auto all_velocities() -> std::vector<VIPRA::f3d> const&;

void update(VIPRA::State const&);
```

# Input Implementation

> !!! Important Note: Inputs should hold off on loading their input until their `load` method is called. This reduces the amount of I/O during parallel runs.

### Interface

Along with the [Base Module](#base-module-implementation) interface, Input modules are required to implement the following methods.

```C++

void load();

template <typename data_t, typename... key_ts>
auto get(key_ts const&...) -> data_t;
```

#### Serializable Input Implementation

Serializable Input modules additionally require:

```C++
auto serialize() -> std::string;
void deserialize(std::string const&);
```

#### Parameter Input Implementation

Parameter Input modules additionally require:

```C++
template <typename data_t, typename... key_ts>
auto get<VIPRA::Parameter<data_t>>(key_ts const&...) -> std::optional<VIPRA::Parameter<data_t>>;
```

<!-- TODO: add in how the values should be handled -->

#### Polygon Input Implementation

Polygon Input modules additionally require:

```C++
template <typename... key_ts>
auto load_polygons(key_ts const&...) -> std::optional<std::vector<VIPRA::Geometry::Polygon>>;
```

# Output Implementation

### Interface

Along with the [Base Module](#base-module-implementation) interface, Output modules are required to implement the following methods.

Helper macros can be found in [Output Macros](#output-macros)

```C++
auto write();

template <typename value_t>
void timestep_value(std::string const&, VIPRA::timestep, value_t);

template <typename value_t>
void ped_timestep_value(VIPRA::idx, VIPRA::timestep, std::string const&, value_t);


```