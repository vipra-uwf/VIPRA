
# Module

Each module type has its own requirements, these are the requirements common to all.

# File / Naming Requirements

For the module to be correctly identified by [vipra.py](../../researchers/usage.md)

1. The module files must be placed under `modules/*type*/*module name*`
2. The module .hpp file must be named *module name*.hpp
3. The module class name and VIPRA_MODULE_NAME() must match the .hpp *module name*
4. The module class must be under the *type* namespace
5. A `CMakeLists.txt` must be placed in the module directory

- Note: 3 & 4 are covered when using the `NEW_VIPRA_MODULE(*name*, *type*)` macro

[The CMakeLists.txt requirements are at the bottom](#CMakeLists)

# Class Requirements

The module class is required to have the following. Each is explained in more details below.

```C++
VIPRA_MODULE_TYPE(type)
VIPRA_MODULE_NAME("name")
VIPRA_REGISTER_PARAMS {}
```

# Details

```C++
VIPRA_MODULE_TYPE(type)
```

Possible Choices:
```C++
Model,
Goals,
Input,
Output,
Pedestrians,
Map,
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
# If searching locally
vipra_add_library(
  LIBNAME *name*
  LINK    *library name for linker, optional*
  TURN_ON *list of options to turn on, optional*
  TURN_OFF *list of options to turn off, optional*
)

# If pulling a .tar file
vipra_add_library(
  LIBNAME *name*
  LIBURL  *url*
  LINK    *library name for linker, optional*
  TURN_ON *list of options to turn on, optional*
  TURN_OFF *list of options to turn off, optional*
)

# If pulling from git
vipra_add_library(
  LIBNAME *name*
  GIT_URL *url*
  GIT_TAG *tag, optional*
  LINK    *library name for linker, optional*
  TURN_ON *list of options to turn on, optional*
  TURN_OFF *list of options to turn off, optional*
)
```

`LINK` is the name used by the linker (`-l*LINK*`)  it defaults equal to `LIBNAME`.

> !!! Important Note: vipra_add_library may not work for some libraries, in that case add the library manually through normal CMake commands. Let us know of any libraries that don't work.

### Example

This example is from the [JSON Input Module](../../researchers/base_modules.md)

```CMake
vipra_add_all()

vipra_add_library(
  LIBNAME nlohmann_json
  URL https://github.com/nlohmann/json.git
)
```