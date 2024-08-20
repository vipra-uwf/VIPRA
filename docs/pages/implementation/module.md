
# Module

All modules are required to have the following:

# Required

```C++
VIPRA_MODULE_TYPE(type)
VIPRA_MODULE_NAME("name")
VIPRA_REGISTER_PARAMS

// Also require a seperate file to go along with source
CMakeLists.txt
```

[The CMakeLists.txt is at the bottom](#CMakeLists)


# Details

```C++
VIPRA_MODULE_TYPE(type)
```

Possible Choices:
```C++
MODEL,
GOALS,
INPUT,
OUTPUT,
PEDESTRIANS,
OBSTACLES,
```

## Effects

This sets the type of the module, making sure it receives the proper parameters and is used correctly.

---

```C++
VIPRA_MODULE_NAME("name")
```

## Effects

This sets the name of the module, making sure it receives the proper parameters

---

```C++
VIPRA_REGISTER_PARAMS {}
```

## Effects

This adds parameters to the module that are loaded in at the start of a simulation

## Example:

```C++
// Example Member variables that will hold the parameter values
int param1;
std::string param2;

VIPRA_REGISTER_PARAMS {
  VIPRA_PARAM("param1", param1),
  VIPRA_PARAM("param2", param2)
}
```

---

# CMakeLists.txt

A CMakeLists.txt file tells the build system how to include the module using [CMake](https://cmake.org/)

Most modules simply require the following:
```CMake
vipra_add_all()
```

This will add all source and header files to the VIPRA project.

### Adding External Libraries

Additionally, if any external libraries are needed they can be added with:
```CMake
vipra_add_library(
  LIBNAME *name*
  LIBURL *url*
)
```
with `liburl` being an optional url to pull the library from.

If the url is not provided CMake will search for the library locally.

> !!! Important Note: vipra_add_library may not work for some libraries, in that case add the library manually through normal CMake commands. Let us know of any libraries that don't work.

### Example

This example is from the [JSON Input Module](../base_modules.md)

```CMake
vipra_add_all()

vipra_add_library(
  LIBNAME nlohmann_json
  URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
)
```