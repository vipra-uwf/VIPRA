
| [Main Page](../../docs.md) | [How To Use](usage.md) | [Provided Modules](base_modules.md) | [VIPRA Behaviors](behaviors/behaviors.md) | [Module Parameters](parameters.md) | [Developing Modules](../developers/overview.md) |

# Modules

The VIPRA framework is designed to be customizable to individual needs. 
This is done through `Modules`, each module is responsible for different parts of a simulation.

[Developers can extend VIPRA through `Modules`](../developers/overview.md).

## Module Types

Modules can be categorized into 6 different types.
- `Model`
- `Goals`
- `Map`
- `Pedestrian Set`
- `Input`
- `Output`

Some [Base Module Implementations](base_modules.md) are provided with VIPRA 

---

### Model

`Model` modules are the pedestrian dynamics models that govern how the pedestrians will move during a simulation. 

---

### Goals

Goals modules handle pathfinding for the simulation.

---

### Map

Map modules hold the geometry of the environment and handle map/pedestrian collision detection.

The main focus of Map modules is performance.

---

### Pedestrian Set

Pedestrian Set modules hold the pedestrian positions/velocities and handle queries to pedestrians (ex. nearest neighbor).

The main focus of the pedestrian set is performance, but can also be used to add constraints to how pedestrians can move (i.e. celluar automata vs continuous fields)

---

### Input

Input modules handle loading input for the simulation.

These include the: 
- `Map Input`: responsible for loading in the map file
- `Pedestrian Input`: responsible for loading in initial pedestrian positions

---

### Output

Output modules handle producing output from the simulation.

Multiple can be added to one simulation to produce.
