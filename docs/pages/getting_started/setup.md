---
title: Setup
layout: template
filename: setup.md
--- 

# Setup

VIPRA can either be used as a [Library](#library) for use in other projects or built as a [Standalone Simulation](#standalone-build).

There is an [Example Simulation](../examples/simulation.md).

## Standalone Build

The `VIPRA` repository comes with a ready `main.cpp` that follows the [Example Simulation](../examples/simulation.md).

To build the simulation, in the parent directory run:
```
make
```

The [Example Simulation](../examples/simulation.md) provided can be run with:
```
./VIPRA
```

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

