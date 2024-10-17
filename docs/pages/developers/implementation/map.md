
# Map Implementation

Map modules hold the geometry of the environment and handle map/pedestrian collision detection.

# Required Methods

In addition to the [Base Module](module.md) requirements `Goals` require the following:

```C++
VIPRA_MAP_INIT

std::vector<std::string> const& get_object_types_impl();
std::vector<VIPRA::f3d> const& get_objects_impl(std::string const& type);

bool collision_impl(VIPRA:f3d point);
bool collision_impl(VIPRA::Geometry::Circle area);
VIPRA::f_pnt ray_hit_impl(VIPRA::f3d start, VIPRA::f3d end);

VIPRA::f3d get_dimensions_impl() const;
```

# Details


```C++
VIPRA_MAP_INIT
```

## Parameters

<!-- TODO: update this -->

- `obstacles` : std::vector<[VIPRA::Polygon](../usage/types.md)> - vector containing the VIPRA::Polygons for each obstacle
- `types` : std::vector<std::string> - vector containing a list of the types of objects in the map
- `objects` : std::map<std::string, VIPRA::f3dVec> - map containing each object type and the positions of each of the objects of those types

## Expected Effects

The Map is ready for the first timestep of the simulation

---

```C++
std::vector<std::string> const& get_object_types_impl();
```

## Parameters

`NONE`

## Returns

The list of object types in the map

## Expected Effects

`NONE`

---

```C++
std::vector<VIPRA::f3d> const& get_objects_impl(std::string const& type);
```

## Parameters

- `type` - the type of object to get the positions of

## Returns

vector containing the positions of the objects of the provided type

## Expected Effects

`NONE`

---

```C++
bool collision_impl(VIPRA:f3d point);
```

## Parameters

- `point` - the point to check for collision with an obstacle

## Returns

- `true` if the point is within an obstacle
- `false` if not

## Expected Effects

`NONE`

---

```C++
bool collision_impl(VIPRA::Geometry::Circle area);
```

## Parameters

- `area` - the circle to check for collision with an obstacle

## Returns

- `true` if any point inside the circle is also within an obstacle
- `false` if not

## Expected Effects

`NONE`

---

```C++
VIPRA::f_pnt ray_hit_impl(VIPRA::f3d start, VIPRA::f3d end);
```

## Parameters

- `start` - the start of the line segment
- `end` - the end of the line segment

## Returns

- The distance from `start` to the collision point, if the ray collides with an obstacle
- -1 if not

## Expected Effects

`NONE`

---

```C++
VIPRA::f3d get_dimensions_impl() const;
```

## Parameters

`NONE`

## Returns

The X, Y, and Z dimensions of the map

## Expected Effects

`NONE`

---

