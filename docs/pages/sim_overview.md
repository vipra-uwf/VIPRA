---
title: Simulation Overview
layout: template
filename: sim_overview.md
--- 

## Simulation

### Simulation Steps

These are the general steps taken when a simulation is run.

#### 1. VIPRA::simulation

First a Simulation is constructed using the `VIPRA::simulation` utility method.

#### 2. Run Sim

The simulation is started using either `operator()` or `run_sim()`.

These both take
1. [`Input Module`](modules/input.md) - Loads Pedestrians
2. [`Polygon Input Module`](modules/input.md) - Loads Obstacles
3. [`Parameter Module`](module/input.md) - Loads Parameters

as their arguments.

#### 3. Parameters Loaded

The [`Parameter Module`](modules/special_modules.md) calls `load()` on it's [`Parameter Input`](modules/input.md) module.

#### 4. Parameters Registered

Each Module has its [REGISTER_MODULE_PARAMS](modules.md) method called.

This is used to tell the simulation which parameters are required by each module and where to place them.

#### 5. Modules Configured

The loaded parameters are passed in to each module.

#### 6. Modules Initialized

The [`Obstacles`](modules/obstacles.md), [`Goals`](modules/goals.md), [`Model`](modules/model.md), and [`Behavior Model`](behaviors/behavior_model.md) have their `INIT_STEP` methods called.

This allows each module to setup any necessary pre-simulation work.

#### 7. Simulation Run

The following is run until the [`Goals`](modules/goals.md) module simulation goal is met, or until the simulation `max_timestep` is reached.

```
model.timestep();
behaviors.timestep();
pedestrian_set.update();
goals.update();
```

#### 8. Output

Finally, each of the [`Output`](modules/output.md) modules `write` method is called.