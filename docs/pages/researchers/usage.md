
| [Main Page](../../docs.md) | [How To Use](usage.md) | [Provided Modules](base_modules.md) | [VIPRA Behaviors](behaviors/behaviors.md) | [Module Parameters](parameters.md) | [Developing Modules](../developers/overview.md) |
| --- | --- | --- | --- | --- | --- |

# VIPRA Usage

# Requirements

The following are requirement for running the VIPRA simulation.

1. [CMake](https://cmake.org/download/) version 3.14 or newer
4. [Python](https://www.python.org/) 3.10 or newer
2. A compiler that supports C++20
3. Linux

Additionally, for distributed simulations:

3. MPI, currently [openMPI](https://www.open-mpi.org/software/ompi/v5.0/) is known to work

# Creating A Simulation

A [Python](https://www.python.org/) script is provided for easily setting up a simulation.

Run the following in the main directory:
```
python3 vipra.py -n <name>
```

This will start a setup process in which the user chooses which modules to use for the simulation.
The options for each are provided at each step.

A description of each module type can be found in the [Modules Page](modules.md)

After the configuration step is done run:
```
python3 vipra.py -c -n <name>
```

## Running The Simulation

After the simulation has been created it can be run with:

```
python3 vipra.py -r -n <name>
```

## Recreating The Simulation

If something has changed with one of the modules the simulation can be updated with:

```
python3 vipra.py -c -n <name>
```

# Simulation Parameters

Parameters are loaded into the simulation from a JSON file; the path to which is asked for when running with the `-r` flag.

Each module implementation has its own set of parameters.

An example set of parameters is provided in the `examples/module_params.json` file.

## Parameter Sweeps

Parameters can be randomized, in parallel or serial, to get a wide range of possible outcomes.

See more on the [Parameter Sweeps Page](parameter_sweeps.md)

# VIPRA Behaviors

A detailed explanation of behaviors can be found in the [Behaviors Page](behaviors/behaviors.md)

The list of behaviors to use in a simulation run are provided in the simulation parameters JSON file.

Example:
```JSON
"behavior_model": {
  "main": {
    "behaviors_dir": "behaviors",
    "behaviors": [
      "disembark"
    ]
  }
}
```

# Visualization

A [very simple visualizer](tools/visualizer.md) is provided for `TrajectoriesJson` output.