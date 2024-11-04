
| [Main Page](../../docs.md) | [How To Use](usage.md) | [Provided Modules](base_modules.md) | [VIPRA Behaviors](behaviors/behaviors.md) | [Module Parameters](parameters.md) | [Developing Modules](../developers/overview.md) |

# Included Modules

There are several modules included with VIPRA.

**Model**:
- [Calm Model](#calm-model)

**Goals**:
- [AStar](#astar-goals)

**Map**:
- [QuadTree](#quadtree-obstalce-set)

**Pedestrian Set**:
- [Grid](#grid-pedestrian-set)

**Input**:
- [JSON](#json-input)

**Output**:
- [JSON](#json-output)

---

# Models

## Calm Model

This `Model` module implements the [Calm Model](https://journals.plos.org/plosone/article?id=10.1371/journal.pone.0229690)

### Parameters:
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
    "calm": {
      "meanMass": 1.0,
      "massStdDev": 0.1,
      "meanReactionTime": 0.5,
      "reactionTimeStdDev": 0.05,
      "meanMaxSpeed": 1.0,
      "meanShoulderLen": 0.2,
      "maxSpeedStdDev": 0.1,
      "shoulderLenStdDev": 0.05
    }
  },
```

---

# Goals

## AStar Goals

This `Goals` module uses the [A* algorithm](usage/algorithms.md) to find paths for each pedestrian.

### Parameters:
- `endGoalType` : Name of object in map each pedestrian tries to reach (e.g. "exit")
- `goalRange` : Range, in meters, before a goal is counted as "reached" (e.g. 0.05)
- `gridSize` : Length of each side, in meters, of each grid in the pathing graph (e.g. 0.1)
- `closestObstacle` : Closest a grid center can be to an obstacle before being considered non-traversable

In **module_params.json**:
```json
  "goals": {
    "AStar": {
      endGoalType: "exit",
      goalRange: 0.1,
      gridSize: 0.1,
      closestObstacle: 0.35
    }
  },
```

---

# Maps

## QuadTree Map 

This `Map` module uses the [Quad Tree](usage/data_structures.md) to hold the map geometry.

### Parameters:

- `minQuadSize` : Size at which quads stop being subdivided

In **module_params.json**:
```json
  "map": {
    "QuadTree": {
      "minQuadSize": 0.05
    }
  },
```

---

# Pedestrian Sets

## Grid Pedestrian Set

This `Pedestrian Set` module holds pedestrians in a grid of cells, allowing for more efficient lookup of nearest neighbors

### Parameters:

- `gridSize`: Size of each grid cell

In **module_params.json**:
```json
  "pedestrians": {
    "Grid": {
      "gridSize": 0.5
    }
  },
```

# Inputs

## JSON Input

This `Input` module loads JSON data, using [nlohmann JSON](https://github.com/nlohmann/json).

For importing maps, it is recommended to use the DXF module. More information on how to use .dxf files in your project can be found [here](maps/maps_tutorial.md).

### Parameters:

- NONE

---

# Outputs

## TrajectoriesJson Output

This `Output` module creates a JSON file containing the positions of each pedestrian for each timestep that matches the output frequency in the simulation module parameters.

### Parameters:

- `filename`: Name of the output file

In **module_params.json**:
```json
  "output": {
    "coordinator": {  // Output is special in that it has a coordinator for when there are multiple output modules
      "output_dir": "./sim_output"
    },
    "TrajectoriesJson": {
      "filename": "trajectories.json"
    }
  },
```

---
