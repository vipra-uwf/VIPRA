
# Input Implementation

> !!! Important Note: Inputs should hold off on loading their input until their `load` method is called. This reduces the amount of I/O during parallel runs.

There are several sub-sets of `Input Modules`:
1. 

# Required Methods

```C++
void load();

template <typename data_t, typename... key_ts>
data_t get(key_ts const&...);
```

# Details

# Serializable Input Implementation

Serializable Input modules additionally require:

```C++
std::string serialize();
void deserialize(std::string const&);
```

# Parameter Input Implementation

Parameter Input modules additionally require:

```C++
template <typename data_t, typename... key_ts>
std::optional<VIPRA::Parameter<data_t>> get<VIPRA::Parameter<data_t>>(key_ts const&...);
```

<!-- TODO: add in how the values should be handled -->

# Polygon Input Implementation

Polygon Input modules additionally require:

```C++
template <typename... key_ts>
std::optional<std::vector<VIPRA::Geometry::Polygon>> load_polygons(key_ts const&...);
```