
| [Main Page](../../docs.md) | [How To Use](usage.md) | [Provided Modules](base_modules.md) | [VIPRA Behaviors](behaviors/behaviors.md) | [Module Parameters](parameters.md) | [Developing Modules](../developers/overview.md) |

# VIPRA Usage

### Table of Contents
> [Requirements](#requirements)  
[Creating a Simulation](#creating-a-simulation)  
[Running the Simulation](#running-the-simulation)    
\- [Flags](#flags)   
\- [Simulation Parameters](#simulation-parameters)   
\- [Simulation Config](#simulation-config)   
\- [Parameter Sweeps](#parameter-sweeps)   
\- [Maps](#maps)   
[VIPRA Behaviors](#vipra-behaviors)   
[Visualization](#visualization)   

# Requirements

The following are requirement for running the VIPRA simulation.

1. [CMake](https://cmake.org/download/) version 3.14 or newer
2. A compiler that supports C++20
3. Linux

Additionally, for distributed simulations:

3. MPI, currently [openMPI](https://www.open-mpi.org/software/ompi/v5.0/) is known to work

# Creating A Simulation

The simulation and any modules are compiled by running:
```
make
```

To allow for MPI:
```
make mpi=on
```

# Running The Simulation

The simulation is run with
```
./VIPRA <flags>
```

## Flags

- `-map=<path>`: path to the map file (required)
- `-peds=<path>`: path to the pedestrian initial positions
- `-params=<path>`: path to the parameter file, default = ./module_params.json
- `-modules=<path>`: path to the sim config file, default = ./sim_config.json
- `-count=<number>`: number of simulations to run, default = 1 (more on this in [Parameter Sweeps](parameter_sweeps.md))
- `-install=<path>`: path to module install directory, default = ./install
- `-log=<debug|info|warn|error>`: minimum level to log, default = info

## Simulation Parameters

Parameters are loaded into the simulation from a JSON file, each module implementation has its own set of parameters.

An example set of parameters is provided in the `examples/module_params.json` file.

## Simulation Config

A simulation config file is a JSON file that tells VIPRA which modules to use.

The types of modules are discussed in: [Modules](modules.md)

An example simulation config is provided in the `examples/sim_config.json` file.

## Parameter Sweeps

Parameters can be randomized, in parallel or serial, to get a wide range of possible outcomes.

See more on the [Parameter Sweeps Page](parameter_sweeps.md)

## Maps

A map defineds the geometry and special areas used by the simulator. These maps include obstacles, spawn positions, objectives for agents to use as goals, and special areas that change agent behavior. 

See more on the [Maps Tutorial](maps/maps_tutorial.md) page

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