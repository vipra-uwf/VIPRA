
| [Main Page](../../docs.md) | [Developer Setup](setup.md) | [Module Implementations](overview.md) | [Examples](examples/list.md) |

VIPRA can either be used:
 - as a [Library](library.md) for use in other projects
 - as a [Standalone Simulation](pages/researchers/usage.md)

# Setup

Requirements and Setup are found on the [Setup Page](setup.md)

# Modules

`VIPRA` is broken into `Modules`, each with their own responsibilities in the simulation

Here are the requirements for each of the module types.

1. [Base Module](./implementation/module.md) - Requirements common to all module types

2. [Model](./implementation/model.md)

3. [Goals](./implementation/goals.md)

4. [Maps](./implementation/map.md)

5. [Pedestrians](./implementation/pedestrians.md)

6. [Input](./implementation/input.md)

7. [Output](./implementation/output.md)

---

# Simulation Overview

Below are the steps taken in a full simulation run.

<details>
<summary>Simulation Steps</summary>

# Simulation Steps

These are the general steps taken when a simulation is run.

#### 1. Module Loading

First a Simulation is constructed, then Modules are loaded using dlopen.

#### 2. Run Sim

The simulation is started using either `operator()` or `run_sim()`.

These both take
1. [`Input Module`](modules/input.md) - Loads Pedestrians
2. [`Map Input Module`](modules/input.md) - Loads Map geometry
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

The [`Map`](modules/map.md), [`Goals`](modules/goals.md), [`Model`](modules/model.md), and [`Behavior Model`](behaviors/behavior_model.md) have their `INIT_STEP` methods called.

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
</details>

