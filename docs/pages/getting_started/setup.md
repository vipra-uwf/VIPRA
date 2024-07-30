**File Path:** [Home](../../docs.md) / [Setup](setup.md)
# Setup

VIPRA can either be used as a [Library](#library) for use in other projects or built as a [Standalone Simulation](#standalone-build).

An example of how to use the simulation function is provided here: [Example Simulation](../examples/simulation.md).

# Standalone Build

The `VIPRA` repository comes with a ready `main.cpp` that follows the [Example Simulation](../examples/simulation.md).

This build takes one argument, the number of simulations to run, and generates a number of "trajectories.json" files based on that parameter. This output file can then be used with the [visualizer program](../tools/visualizer.md) also included in this project to generate a visual representation of pedestrians in motion. 

## Requirements

The following are requirement for running the VIPRA simulation.

1. [CMake](https://cmake.org/download/) version 3.22.2 or newer.
2. A compiler that can run C++20 or newer.
3. [OpenMPI](https://www.open-mpi.org/software/ompi/v5.0/) version 5.05 or newer.

The following are required for development.

1. ANTLR4 - version 4.13.1 or newer.

---

## Build

To build the simulation, in the parent directory run:
```
make
```

Alternatively, run one of the following (recommended for developers only):

- `make release` for compiling in release mode.
- `make debug` for compiling in debug mode.
- `make perf` for compiling the performance testing version.

---

## Running the Example

The [Example Simulation](../examples/simulation.md) provided can be run with:
```
./VIPRA (optional)<number_of_simulations>
```

See the [Usage Page](usage.md) for how to use VIPRA as a standalone program

---

## Library

`VIPRA` can be included in a project using [CMake](https://cmake.org/)

```
target_link_libraries(
  **project**
  PUBLIC
  VIPRA
)
```

- Replace `**project**` with the name of your CMake project

---

[Return to main page](../../docs.md)