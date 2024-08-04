**File Path:** [Home](../docs.md) / [Researchers](researchers.md) / [Modules](modules.md)
# Modules

The VIPRA framework is designed to be customizable to individual needs, Developers can extend VIPRA through `Modules`.

Each `Module` has its respective [Parameters](parameters.md) that are loaded in for each simulation run.

## Module Types

Modules can be categorized into 7 different types. A simulation requires all 7 types of modules to function properly. 

### Model

[Model](modules/model.md) modules are the pedestrian dynamics models that govern how the pedestrians will move during a simulation. 

For an example of a Model module, see the included [Calm Model module](base_modules.md#calm-model).

### Goals

[Goals](modules/goals.md) modules handle pathfinding for the simulation.

### Obstacle Set

[Obstacle Set](modules/obstacles.md) modules hold the geometry of the environment and handle map/pedestrian collision detection.


### Pedestrian Set

[Pedestrian Set](modules/pedestrians.md) modules hold the pedestrian positions/velocities and handle queries to pedestrians (ex. nearest neighbor).


### Input

[Input](modules/input.md) modules handle loading input for the simulation. There are several specialized input types:
- [Serializable Input](modules/input.md#seiralizable-input) modules are inputs that allow for serialization of their values. Needed for [Parameter Sweep](parameter_sweep.md) runs.
- [Parameter Input](modules/input.md#parameter-input) modules are able to load parameters
- [Polygon Input](modules/input.md#polygon-input) modules are able to load polygons for use in obstacle sets


### Output

[Output](modules/output.md) modules handle producing output from the simulation


### Output Coordinator

The [Output Coordinator](modules/special_mdules.md) module handles coordinating any number of [Output](modules/output.md) modules.


## How to Change Modules

TODO(tyler): Clear instructions on how to swap out modules.
