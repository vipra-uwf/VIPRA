
**File Path:** [Home](../docs.md) / [Researchers](researchers.md) / [Parameter Sweep](parameter_sweep.md)

# Parameter Sweep

The VIPRA Parameter sweep allows the user to run a randomized parameter sweep. This can be done on a single machine or a cluster, through the use of [openMPI](https://www.open-mpi.org/)

# Use

A [Simple Parameter Sweep Example](examples/parameter_sweep.md) is provided.

The provided `main.cpp` also implements a parameter sweep.

### Building with MPI

`MPI` is enabled with the addition of `mpi=on` to the make command

Example:
```
make release mpi=on
```

Without this, the parameter sweep with be run serially on one core.

> !!! IMPORTANT NOTE: currently only [openMPI](https://www.open-mpi.org/) is known to work, this will be fixed in the future

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