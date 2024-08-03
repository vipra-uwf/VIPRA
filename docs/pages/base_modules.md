
**File Path:** [Home](../docs.md) / [Researchers](researchers.md) / [Included Modules](base_modules.md)
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

In **main.cpp**:
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

In **module_params.json**:
```json
  "model": {
    "calm_model": {
      "meanMass": 1.0,
      "massStdDev": [
        0.1,
        0.2,
        0.15
      ],
      "meanReactionTime": {
        "min": 0.4,
        "max": 0.6
      },
      "reactionTimeStdDev": 0.05,
      "meanMaxSpeed": 1.0,
      "meanShoulderLen": 0.2,
      "maxSpeedStdDev": 0.1,
      "shoulderLenStdDev": 0.05
    }
  },
```

### AStar Goals

This `Goals` module uses the [A* algorithm](usage/algorithms.md) to find paths for each pedestrian.

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

This `Obstacle Set` module uses the [Quad Tree](usage/data_structures.md) to hold the map geometry.

#### Use:
```C++
#include "vipra.hpp"

VIPRA::simulation(
  ...
    VIPRA::Obstacles::QuadTree{} // Obstacle sets go inside the Map module
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

sim(
  VIPRA::Input::JSON{"pedestrians.json"}, // Pedestrian Input
  VIPRA::Input::JSON{"obstacles.json"},   // Obstacle Input
  VIPRA::Parameters{VIPRA::Input::JSON{"module_params.json"}} // Parameter Input
);
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
