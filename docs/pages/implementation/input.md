---
title: Input Module Implementation
layout: template
filename: input.md
--- 

# Input Implementation

> !!! Important Note: Inputs should hold off on loading their input until their `load` method is called. This reduces the amount of I/O during parallel runs.

### Interface

<!-- TODO: move relavent macros here -->

```C++

void load();

template <typename data_t, typename... key_ts>
auto get(key_ts const&...) -> data_t;
```

#### Serializable Input Implementation

Serializable Input modules additionally require:

```C++
auto serialize() -> std::string;
void deserialize(std::string const&);
```

#### Parameter Input Implementation

Parameter Input modules additionally require:

```C++
template <typename data_t, typename... key_ts>
auto get<VIPRA::Parameter<data_t>>(key_ts const&...) -> std::optional<VIPRA::Parameter<data_t>>;
```

<!-- TODO: add in how the values should be handled -->

#### Polygon Input Implementation

Polygon Input modules additionally require:

```C++
template <typename... key_ts>
auto load_polygons(key_ts const&...) -> std::optional<std::vector<VIPRA::Geometry::Polygon>>;
```