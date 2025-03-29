|
| [Main Page](../../docs.md) | [Developer Setup](setup.md) | [Module Implementations](overview.md) | [Examples](examples/list.md) |

# Developer Setup

VIPRA can either be used:
 - as a [Library](#library) for use in other projects
 - as a [Standalone Simulation](#standalone-build)

## Requirements

The following are requirement for running the VIPRA simulation.

1. [CMake](https://cmake.org/download/) version 3.14 or newer.
2. A compiler that supports C++20.

Additionally for Parameter Sweeps:
3. MPI, either [openMPI](https://www.open-mpi.org/software/ompi/v5.0/) or [mpich](https://www.mpich.org/)

The following are required for development of the Behaviors DSL.

1. ANTLR4 - version 4.13.1 or newer.

---

# Library

`VIPRA` can be included in a project using [CMake](https://cmake.org/)

```
target_link_libraries(
  **project**
  PUBLIC
  vipra
)
```

- Replace `**project**` with the name of your CMake project

---

# Standalone Simulation

A MakeFile is provided with the proper command to build a standalone simulation from the provided `main.cpp` in the top level directory.

To build the simulation, in the parent directory run:
```
make
```

Alternatively, run one of the following:

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

# Running the Provided Simulation

The VIPRA repo comes with a `main.cpp` that follows the [Parameter Sweep Example Simulation](../examples/parameter_sweep.md)

```
./VIPRA -count=<sim count> -map=<map file path> -peds=<pedestrian file path> -params=<parameters file path>
```

Example:
```
./VIPRA -count=1 -map=maps/obstacles/a320/a320_polygons.json -peds=maps/pedestrians/a320/a320_144_pedestrians.json -params=examples/module_params.json
```

---

[Return to main page](../../docs.md)