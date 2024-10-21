| [Main Page](../../docs.md) | [How To Use](usage.md) | [Provided Modules](base_modules.md) | [VIPRA Behaviors](behaviors/behaviors.md) | [Module Parameters](parameters.md) | [Developing Modules](../developers/overview.md) |


# Module Parameters

Parameters are loaded into each module before they are initizalized. The exact steps of a simulation run are shown in [`Simluation Steps`](../developers/overview.md#simulation-steps).

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

# Parameter Sweeps

Parameter sweeps run many simulations with randomized parameters to get a better idea of the possible outcomes.

# Parameters

Parameters are provided as they are with a normal run, with some extra rules.

`NOTE:` parameters are randomized in a non-parameter sweep run if provided in this format

## Non-Random Values

Any parameter that should not be randomized is provided as it is normally.

```JSON
single_string: "example1"
single_float: 1.0
```

Array's can still be used as they normally would be.

```JSON
array_param: [1.0, 2.0, 3.0]
```

## Discrete Random Values

Parameters can be randomized between a set of discrete values.

For each simulation run, the actual value for the parameter is randomly chosen from the list given with a uniform distribution.

Examples:
```JSON
discrete_string_param: [ "option1", "option2", "option3" ]
discrete_float_param: [ 1.0, 1.2, 1.4 ]
```

`NOTE:` Array parameters can NOT be randomized

```JSON
// will NOT work
random_array_param: [ [1,2], [3,4] ]
```

## Continuous Random Values

Numerical parameters can also be randomized within a continuous range.

For each simulation run, the actual value for the parameter is randomly chosen with a uniform distribution from `min` to `max` inclusively.

Examples:
```JSON
continuous_param: {
  "min": 1.0,
  "max": 4.0
}
```

`NOTE:` Only float values can be continuous