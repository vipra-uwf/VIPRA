
# Developer Setup

VIPRA can either be used:
 - as a [Library](#library) for use in other projects
 - as a [Standalone Simulation](#standalone-build)

# Standalone Simulation

The `VIPRA` repository comes with a ready `main.cpp` that follows the [Example Simulation](../examples/simulation.md).

This build takes one argument, the number of simulations to run, and generates a number of "trajectories.json" files based on that parameter. This output file can then be used with the [visualizer program](../tools/visualizer.md) also included in this project to generate a visual representation of pedestrians in motion. 

## Requirements

The following are requirement for running the VIPRA simulation.

1. [CMake](https://cmake.org/download/) version 3.14 or newer.
2. A compiler that supports C++20.

Additionally for Parameter Sweeps:
3. MPI, either [openMPI](https://www.open-mpi.org/software/ompi/v5.0/) or [mpich](https://www.mpich.org/)

The following are required for development of the Behaviors DSL.

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

## Using MPI

`MPI` is enabled with the addition of `mpi=on` to the make command

Example:
```
make release mpi=on
```

> !!! IMPORTANT NOTE: make sure that the version of MPI you are compiling with matches the version you run the simulation with.

## Using Gperftools

[Gperftools](https://github.com/gperftools/gperftools) can be used with `make perf gperf=on`

---

## Running the Provided Simulation

The VIPRA repo comes with a `main.cpp` that follows the [Parameter Sweep Example Simulation](../examples/parameter_sweep.md)

```
./VIPRA <number_of_simulations>
```

See the [Usage Page](usage.md) for how to modify the simulation.

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