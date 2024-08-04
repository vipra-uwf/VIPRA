
# Module

All modules are required to have the following:

# Required

```C++
VIPRA_MODULE_TYPE(type)
VIPRA_MODULE_NAME("name")
VIPRA_REGISTER_PARAMS
```

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
