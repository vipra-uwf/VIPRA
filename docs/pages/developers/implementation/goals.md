
# Goals Modules

# Required Methods

In addition to the [Base Module](module.md) requirements `Goals` require the following:

```C++
  VIPRA_GOALS_INIT_STEP {}
  VIPRA_GOALS_UPDATE_STEP {}
  VIPRA_GOALS_NEXT_GOAL {}
  VIPRA_GOALS_CHANGE_GOAL {}
```

# Required Method Details

```C++
VIPRA_GOALS_INIT_STEP {}

// aka

void init_step(VIPRA::Modules::Pedestrians const& pedset, VIPRA::Modules::Map const& map, VIPRA::Random::Engine& engine);
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
VIPRA_GOALS_UPDATE_STEP {}

// aka

void update_step(VIPRA::Modules::Pedestrians const& pedset, VIPRA::Modules::Map const& map, VIPRA::delta_t deltaT);
```

Called each timestep

### Parameters

- `pedset` - [Pedestrians module implementation](../modules/pedestrians.md)
- `obstalces` - [Obstalces module implementation](../modules/obstalces.md)
- `deltaT` : VIPRA::f_pnt - amount of time since last time step

### Returns

`NONE`

### Expected Effects

Each pedestrian has their current goals updated, if not already handled in VIPRA_GOALS_NEXT_GOAL

---

```C++
VIPRA_GOALS_NEXT_GOAL

// aka

bool next_goal(VIPRA::idx pedIdx, VIPRA::Modules::Pedestrians const& pedset, VIPRA::Modules::Map const& map, VIPRA::delta_t deltaT);
```

Called when a pedestrian reaches their current goal

### Parameters

- `pedIdx` : VIPRA::idx - ID of pedestrian that has reached their current goal
- `pedset` - [Pedestrians module implementation](../modules/pedestrians.md)
- `obstalces` - [Obstalces module implementation](../modules/obstalces.md)
- `deltaT` : VIPRA::f_pnt - amount of time since last time step

### Returns

`bool`: true if the pedestrian has reached their final goal, false if not

### Expected Effects

Pedestrian # pedIdx has their current goal updated, if not already handled in VIPRA_GOALS_UPDATE_STEP

---

```C++
VIPRA_GOALS_CHANGE_GOAL

// aka

void change_end_goal(VIPRA::idx pedIdx, VIPRA::f3d currPos, VIPRA::f3d goalPos);
```

Called from Behaviors

### Parameters

- `pedIdx` : VIPRA::idx - ID of pedestrian to update
- `currPos` : VIPRA::f3d - the current position of the pedestrian to update
- `goalPos` : VIPRA::f3d - the position of the new end goal

### Returns

`NONE`

### Expected Effects

Pedestrian # pedIdx has their end goal changed to `goalPos`

---
