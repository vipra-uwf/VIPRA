
## Module Parameters

Parameters are loaded into each module before they are initizalized. The exact steps of a simulation run are shown in [`Simluation Steps`](sim_overview.md).

Parameters can use any [`ParaReader`](modules/input.md) module, the [`JSON`](base_modules.md) input module provided works for this.

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

Example of a [`Goals`](modules/goals.md) module parameters:
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

## Parameter Sweep

Parameters can be randomized through a [Parameter Sweep](parameter_sweep.md)