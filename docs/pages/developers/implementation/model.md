# Model

# Required Methods

In addition to the [Base Module](module.md) requirements `Model` modules require the following:

```C++
VIPRA_MODEL_INIT_STEP {}

VIPRA_MODEL_TIMESTEP {}
```

# Details


```
VIPRA_MODEL_INIT_STEP {}
```

- `pedset` - the [Pedestrians Model implementation](../modules/pedestrians.md)
- `map` - the [Map module implementation](../modules/map.md)
- `goals` - the [Goals implementation](../modules/goals.md)
- `output` - the [Output Coordinator](../modules/special_modules.md)
- `engine` - the [Randomization engine](../usage/randomization.md)

### Returns

`NONE`

### Exepected Effects

The pedestrian dynamics model is ready for the first simulation timestep

---

```
VIPRA_MODEL_TIMESTEP {}
```

### Parameters

- `pedset` - the [Pedestrians Model implementation](../modules/pedestrians.md)
- `map` - the [Map module implementation](../modules/map.md)
- `goals` - the [Goals implementation](../modules/goals.md)
- `output` - the [Output Coordinator](../modules/special_modules.md)
- `state` - the [State](../usage/types.md#state) to write the new positions and velocities to
- `deltaT` - the timestep size
- `timestep` - the current timestep number

### Returns

`NONE`

### Expected Effects

`state` contains the updated positions and velocities of each pedestrian

---
