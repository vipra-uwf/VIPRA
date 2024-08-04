
# Goals Modules

# Required Methods

In addition to the [Base Module](module.md) requirements `Goals` require the following:

```C++
  VIPRA_GOALS_INIT_STEP {}
  VIPRA_GOALS_UPDATE_STEP {}

  void                  change_end_goal_impl(VIPRA::idx pedIdx, VIPRA::f3d currPos, VIPRA::f3d goalPos);
  const VIPRA::f3dVec&  current_goals_impl() const;
  const VIPRA::f3dVec&  end_goals_impl() const;
  const VIPRA::f3d&     current_goal_impl(VIPRA::idx pedIdx) const;
  const VIPRA::f3d&     end_goal_impl(VIPRA::idx pedIdx) const;
  bool                  is_goal_met_impl(VIPRA::idx pedIdx) const;
  bool                  is_sim_goal_met_impl() const;
  VIPRA::f_pnt          time_since_last_goal_impl(VIPRA::idx pedIdx) const;
```

# Details

```C++
VIPRA_GOALS_INIT_STEP
```

### Parameters

- `pedset` - [Pedestrians module implementation](../modules/pedestrians.md)
- `obstalces` - [Obstalces module implementation](../modules/obstalces.md)

### Returns

`NONE`

### Expected Effects

Each pedestrian has their goals initialized and ready for the first timestep of the simulation

---

```C++
VIPRA_GOALS_UPDATE_STEP
```

### Parameters

- `pedset` - [Pedestrians module implementation](../modules/pedestrians.md)
- `obstalces` - [Obstalces module implementation](../modules/obstalces.md)
- `deltaT` : VIPRA::f_pnt - amount of time since last time step

### Returns

`NONE`

### Expected Effects

Each pedestrian has their current goals updated 

---

```
void change_end_goal_impl(VIPRA::idx pedIdx, VIPRA::f3d currPos, VIPRA::f3d goalPos);
```

### Parameters

- `pedIdx` : VIPRA::idx - ID of pedestrian to update
- `currPos` : VIPRA::f3d - the current position of the pedestrian to update
- `goalPos` : VIPRA::f3d - the position of the new end goal

### Returns

`NONE`

### Expected Effects

Pedestrian # pedIdx has their end goal changed to `goalPos`

---

```C++
const VIPRA::f3dVec&  current_goals_impl() const;
```

### Parameters

`NONE`

### Returns

`const VIPRA::f3dVec&` - vector containing the point each pedestrian is currently trying to reach

### Expected Effects

`NONE`

---

```C++
const VIPRA::f3dVec&  end_goals_impl() const;
```

### Parameters

`NONE`

### Returns

`const VIPRA::f3dVec&` - vector containing the point of each pedestrians end goal

### Expected Effects

`NONE`

---

```C++
const VIPRA::f3d& current_goal_impl(VIPRA::idx pedIdx) const;
```

### Parameters

`pedIdx` : VIPRA::idx - ID of pedestrian to retreive the current goal of

### Returns

`VIPRA::f3d` - the point the pedestrian with ID `pedIdx` is currently trying to reach

### Expected Effects

`NONE`

---

```C++
const VIPRA::f3d& end_goal_impl(VIPRA::idx pedIdx) const;
```

### Parameters

`pedIdx` : VIPRA::idx - ID of pedestrian to retreive the end goal of

### Returns

`VIPRA::f3d` - the point the pedestrian with ID `pedIdx` is ulimately trying to reach

### Expected Effects

`NONE`

---

```C++
bool is_goal_met_impl(VIPRA::idx pedIdx) const;
```

### Parameters

`pedIdx` : VIPRA::idx - ID of pedestrian to check

### Returns

`bool` - `true` if the pedestrian has met their end goal, `false` if they have not

### Expected Effects

`NONE`

---

```C++
bool is_sim_goal_met_impl() const;
```

### Parameters

`NONE`

### Returns

`bool` - `true` if each of the pedestrian has achieved their goal, `false` if at least one pedestrian has not reached their end

### Expected Effects

`NONE`

---

```C++
VIPRA::f_pnt time_since_last_goal_impl(VIPRA::idx pedIdx) const;
```

### Parameters

`pedIdx` : VIPRA::idx - ID of pedestrian to get time of

### Returns

`VIPRA::f_pnt` - simulated time since the pedestrian with ID `pedIdx` has reached their last goal

### Expected Effects

`NONE`