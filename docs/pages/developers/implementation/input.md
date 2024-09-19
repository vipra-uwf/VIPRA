
# Input Implementation

> !!! Important Note: Inputs should hold off on loading their input until their `load_impl` method is called. This reduces the amount of I/O during parallel runs.

There are several sub-sets of `Input Modules`:
1. [Serializable Input](#serializable-input-implementation)
2. [Map Input](#map-input-implementation)

# Required Methods

```C++
void load_impl();

template <typename data_t>
std::optional<data_t> get(std::vector<std::string> const&);
```


## Details

```C++
void load_impl();
```

### Parameters

`NONE`

### Returns

`NONE`

### Expected Effects

The input is ready to return its loaded data

```C++
template <typename data_t>
std::optional<data_t> get(std::vector<std::string> const&);
```

### Parameters

`keys` - a list of keys that should uniquely identify a requested value

### Returns

`std::optional<data_t>` - an optional with the value, `std::nullopt` if the value was unable to be returned 

### Expected Effects

`NONE`

---

# Serializable Input Implementation

Serializable Input modules additionally require:

```C++
std::string to_string();
void parse_impl(std::string const&);
```

## Details

```C++
std::string to_string()
```

### Parameters

`NONE`

### Returns

`std::string` - the string represenation of the input data, to be used in `deserialize`

### Expected Effects

`NONE`

```C++
void parse_impl(std::string const&);
```

### Parameters

`data` - the serialized data to be deserialized

### Returns

`NONE`

### Expected Effects

The serialized data is deserialized and loaded into the input module

# Map Input Implementation

```C++
std::optional<std::vector<VIPRA::Geometry::Polygon>> get_obstacles_impl() const;
std::optional<std::vector<VIPRA::Geometry::Polygon>> get_spawns_impl() const;
std::optional<std::map<std::string, std::vector<VIPRA::Geometry::Polygon>>> get_objectives_impl() const;
std::optional<std::map<std::string, VIPRA::Geometry::Polygon>> get_areas_impl() const;
```

```C++
std::optional<std::vector<VIPRA::Geometry::Polygon>> get_obstacles_impl() const;
```

### Parameters

`NONE`

### Returns

The map obstacles

- an empty std::vector, if there are no map obstacles
- `std::nullopt`, if there was an issue loading the map obstacles

### Expected Effects

`NONE`

---

```C++
std::optional<std::vector<VIPRA::Geometry::Polygon>> get_spawns_impl() const;
```

### Parameters

`NONE`

### Returns

The map spawns

- an empty std::vector, if there are no map spawns
- `std::nullopt`, if there was an issue loading the map spawns

### Expected Effects

`NONE`

---

```C++
std::optional<std::map<std::string, std::vector<VIPRA::Geometry::Polygon>>> get_objectives_impl() const;
```

### Parameters

`NONE`

### Returns

The map objectives

- an empty std::map, if there are no map objectives
- `std::nullopt`, if there was an issue loading the map objectives

### Expected Effects

`NONE`

---

```C++
std::optional<std::map<std::string, VIPRA::Geometry::Polygon>> get_areas_impl() const;
```

### Parameters

`NONE`

### Returns

The map areas

- an empty std::map, if there are no map areas
- `std::nullopt`, if there was an issue loading the map areas

### Expected Effects

`NONE`

---
