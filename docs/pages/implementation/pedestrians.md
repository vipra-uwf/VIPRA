
# Pedestrian Set Implementation

# Required Methods

In addition to the [Base Module](module.md) requirements `Goals` require the following:

```C++
VIPRA_PEDS_INIT_STEP;
VIPRA_PEDS_UPDATE_STEP;

VIPRA::idx closest_ped() const;
VIPRA::idx conditional_closest_ped() const;
```

# Details

```
VIPRA_PEDS_INIT_STEP;
```

### Parameters

- `input` - [Input module implementation](../modules/input.md)
- `obstacles` - [Obstacle module implementation](../modules/obstacles.md)

### Returns

`NONE`

### Expected Results

The module is ready to correctly return results for its required methods

---

```
VIPRA_PEDS_UPDATE_STEP;
```

### Parameters

- `state` : VIPRA::State - updated positions/velocities of pedestrians

### Returns

`NONE`

### Expected Results

The module is ready to correctly return results for its required methods

---

```
VIPRA::idx closest_ped() const;
```

### Parameters

`NONE`

### Returns

`NONE`

### Expected Results

The module is ready to correctly return results for its required methods

---

```
VIPRA::idx conditional_closest_ped() const;
```

---

