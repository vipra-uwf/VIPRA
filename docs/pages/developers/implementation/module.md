
# Module

Each module type has its own requirements, these are the requirements common to all.

# File Requirements

Every module requires 4 files in a seperate directory

1. module.hpp*
2. module.cpp*
3. CMakeLists.txt
4. vipra.module

*Note: for 1 & 2 the file name can be anything

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
vipra_module(
  NAME
    *name*
  SOURCES
    *list of .cpp files used in module*
  TYPE
    *type*
)
```

- name: being how the module is refered to when selecting which modules to use in the [simulation config](../../researchers/usage.md)
- type: being one of the [module types](../overview.md)

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

```CMake
vipra_module(
  NAME
    Calm
  SOURCES
    calm_model.cpp
  TYPE
    Model
)
```