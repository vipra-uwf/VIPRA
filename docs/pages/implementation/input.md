
# Input Implementation

> !!! Important Note: Inputs should hold off on loading their input until their `load` method is called. This reduces the amount of I/O during parallel runs.

There are several sub-sets of `Input Modules`:
1. 

# Required Methods

```C++
void load();

template <typename data_t, typename... key_ts>
std::optional<data_t> get(key_ts const&...);
```


## Details

```C++
void load();
```

### Parameters

`NONE`

### Returns

`NONE`

### Expected Effects

The input is ready to return its loaded data

```C++
template <typename data_t, typename... key_ts>
std::optional<data_t> get(key_ts const&...);
```

### Parameters

`keys` - a variadic number of keys to identify an input value

### Returns

`std::optional<data_t>` - an optional with the value, `std::nullopt` if the value was unable to be returned 

### Expected Effects

`NONE`

# Serializable Input Implementation

Serializable Input modules additionally require:

```C++
std::string serialize();
void deserialize(std::string const&);
```

## Details

```C++
std::string serialize()
```

### Parameters

`NONE`

### Returns

`std::string` - the string represenation of the input data, to be used in `deserialize`

### Expected Effects

`NONE`

```C++
void deserialize(std::string const&);
```

### Parameters

`data` - the serialized data to be deserialized

### Returns

`NONE`

### Expected Effects

The serialized data is deserialized and loaded into the input module


# Parameter Input Implementation

Parameter Input modules additionally require:

```C++
template <typename data_t, typename... key_ts>
std::optional<VIPRA::Parameter<data_t>> get<VIPRA::Parameter<data_t>>(key_ts const&...);
```

### Parameters

`keys` - a variadic number of keys to identify an input value

### Returns

`std::optional<VIPRA::Parameter<data_t>>` - an optional with a the parameter value, `std::nullopt` if the value was unable to be returned 

### Expected Effects

`NONE`

# Polygon Input Implementation

Polygon Input modules additionally require:

```C++
template <typename... key_ts>
std::optional<std::vector<VIPRA::Geometry::Polygon>> load_polygons(key_ts const&...);
```

### Parameters

`keys` - a variadic number of keys to identify an input value

### Returns

`std::optional<std::vector<VIPRA::Geometry::Polygon>>` - an optional with a vector of polygons described by a map input, `std::nullopt` if the value was unable to be returned 

### Expected Effects

`NONE`