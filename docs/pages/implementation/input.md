
# Input Implementation

> !!! Important Note: Inputs should hold off on loading their input until their `load_impl` method is called. This reduces the amount of I/O during parallel runs.

There are several sub-sets of `Input Modules`:
1. [Serializable Input](#serializable-input-implementation)
2. [Polygon Input](#polygon-input-implementation)

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

# Polygon Input Implementation

Polygon Input modules additionally requires:

`get`: can load values of type: `std::vector<VIPRA::Geometry::Polygon>`