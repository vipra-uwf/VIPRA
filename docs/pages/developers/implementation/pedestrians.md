
# Pedestrian Set Implementation

# Required Methods

In addition to the [Base Module](module.md) requirements `Goals` require the following:

```C++
VIPRA_PEDS_INIT_STEP;
VIPRA_PEDS_UPDATE_STEP;

VIPRA::idx closest_ped(VIPRA::idx pedIdx) const;
VIPRA::idx conditional_closest_ped(VIPRA::idx pedIdx, auto&& condition) const;
```


# Details

```C++
VIPRA_PEDS_INIT_STEP {}
```

### Parameters

- `input` - [Input module implementation](../modules/input.md)
- `map` - [Map module implementation](../modules/map.md)
- `engine` - [VIPRA Random Engine](../usage/randomization.md)

### Returns

`NONE`

### Expected Results

The module is ready for use

---

```C++
VIPRA_PEDS_UPDATE_STEP {}
```

### Parameters

- `state` : VIPRA::State& - out parameter, updated positions/velocities of pedestrians

### Returns

`NONE`

### Expected Results

The module is ready to correctly return results for its required methods

---

```C++
VIPRA::idx closest_ped_impl(VIPRA::idx pedIdx) const;
```

### Parameters

- `pedIdx` : VIPRA::idx - index of pedestrian to find the nearest neighbor of

### Returns

`VIPRA::idx` - index of pedestrian nearest pedestrian pedIdx

### Expected Results

`NONE`

---

```C++
VIPRA::idx conditional_closest_ped_impl(VIPRA::idx pedIdx, auto&& condition) const;
```

### Parameters

- `pedIdx` : VIPRA::idx - index of pedestrian to find the nearest neighbor of
- `condition` : bool(VIPRA::idx) - callable that returns true if the condition matches a given pedestrian

### Returns

`VIPRA::idx` - index of pedestrian nearest pedestrian pedIdx that matches the condition

### Expected Results

`NONE`

---

