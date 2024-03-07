
- [Introduction](#introduction)

- [Usage](#usage) <!-- TODO -->
  - [Simulation Construction](#simulation-construction) <!-- TODO -->
    - [Example Simulation](#example-simulation)
    - [Example Parameter Sweep](#example-parameter-sweep)
    - [Base Modules](#base-modules) <!-- TODO -->
  - [Module Parameters](#module-parameters) <!-- TODO -->

- [Common Errors](#common-errors) <!-- TODO -->

- [Development](#development)
  - [Overview](#overview)
    - [Simulation](#simulation) <!-- TODO -->
    - [Module Types](#module-types)
    - [Parameter Sweep](#parameter-sweeps) <!-- TODO -->
    - [Randomization](#randomization) <!-- TODO -->
    - [VIPRA Types](#vipra-types) <!-- TODO -->

  - [Module Implementation](#module-implementation)
    - [Model Implementation](#model-implementation) <!-- TODO -->
    - [Goals Implementation](#goals-implementation) <!-- TODO -->
    - [Obstacle Set Implementation](#obstacle-set-implementation) <!-- TODO -->
    - [Pedestrian Set Implementation](#pedestrian-set-implementation) <!-- TODO -->
    - [Input Implementation](#input-implementation) <!-- TODO -->
    - [Output Implementation](#output_implementation) <!-- TODO -->

  - [Parameter Sweep Usage](#parameter-sweep-usage) <!-- TODO -->

  - [Macros](#macros) <!-- TODO -->

  - [Utilities](#utilities) <!-- TODO -->
    - [Algorithms](#algorithms) <!-- TODO -->
    - [Data Structures](#data-structures) <!-- TODO -->

# Introduction

This project provides tools for interdisciplinary analysis of fine-scaled pedestrian movement, especially for science-based decision-making. It is motivated by the fact that local movement and interaction patterns of individuals congregated in public locations, such as entertainment venues and transportation hubs, impacts public health in myriad ways. For instance, infectious disease transmission in crowded areas, such as the 2016 measles outbreak in Disney world that resulted in 125 cases, is affected by the evolution of the pedestrian contact network.

In such contexts, mathematical models can be used to explore different “what if” scenarios for planning public health interventions. For example, pedestrian mobility models could help in the design of public spaces and policies that reduce contacts to mitigate disease spread or encourage walking to improve health outcomes. Understanding the fine-scale movement and interaction patterns of people can help design effective policies and spatial layouts to better engineer suitable movement and interaction patterns for improved public health outcomes in several domains.

Pedestrian dynamics enables such analysis by simulating the trajectories of individual pedestrians in a crowd. Such movement is impacted by behavioral characteristics of humans, policy choices of decisions makers, and design decisions regarding the built environment. Furthermore, the impact of pedestrian dynamics is governed by application-domain models, such as infection spread models. Thus, input from a variety of science domains is required to produce comprehensive understanding for science-based decision-making.

This project includes a modular pedestrian dynamics code to which input from different domains could be easily included. In addition, it includes tools to democratize use by users with limited computing background by including a domain specific language to specify human behavior and policy. We also separately provide a recommender system, as a complementary project, to suggested suitable models to use.

---

# Usage

VIPRA is a template framework this means it requires the user to define their own main and handle any command line arguments.

A basic skeleton will be provided in the future for ease of use.

## Simulation Construction

Simulations are created by choosing which implementations to use for the several different [Module Types](#module-types).

A utility function for creating simulations is provided that allows the modules to be passed in in any order:
```C++
template <typename... module_ts>
auto VIPRA::simulation(module_ts&&...) -> VIPRA::SimType;
```

There is an [Example Simulation](#example-simulation) below.

### Required Modules

Each simulation requires the following modules:
- `Model`
- `Goals`
- `Pedestrian Set`
- `Map`
  - `Obstacle Set`
  - `Polygon Input`
- `Output Coordinator`
  - `Output`

`Maps` and `Output Coordinators` are special in that they require modules themselves (denoted by the additional indent)


## Base Modules

There are several base modules included with VIPRA.

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

### QuadTree Obstalce Set 

This `Obstacle Set` module uses the [Quad Tree](#quadtree-datastructure) to hold the map geometry.

#### Use:
```C++
#include "vipra.hpp"

VIPRA::simulation(
  ...
  VIPRA::Module::Map{
    ...
    VIPRA::Obstacles::QuadTree{} // Obstacle sets go inside the Map module
    ...
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

### JSON Output
<!-- TODO -->


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

  // Run the simulation
  sim((VIPRA::Parameters{VIPRA::Input::JSON{"module_params.json"}}));
}
```

## Example Parameter Sweep

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

Parameters are loaded into each module before they are initizalized. The exact steps are showing in [`Simluation Steps`](#simulation-steps). 

Parameters can use any [`Parameter Input`](#parameter-input-implementation) module, by default a [`JSON`](#json-input) input module is provided.

The format for which is:
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

# Common Errors

<!-- TODO -->

---

# Development

# Overview

<!-- TODO -->

## Simulation

### Simulation Steps

<!-- TODO -->

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

# Base Module Implementation

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
void initialize(std::vector<VIPRA::Geometry::Polygon> const&,std::vector<std::string> const&,std::map<std::string, std::vector<VIPRA::f3d>> const&);

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