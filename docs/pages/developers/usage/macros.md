# Macros

For reference on how to use these macros, you can look at `modules/model/calm_model/calm_model.hpp`

## Base Module Macros

#### VIPRA_MODULE_TYPE(type)

`VIPRA_MODULE_TYPE` sets the modules type
- Model,
- Goals,
- Input,
- Output,
- Pedestrians,
- Map,

```C++
VIPRA_MODULE_TYPE(MODEL)
```

#### VIPRA_MODULE_NAME(name)

`VIPRA_MODULE_NAME` sets the modules name, the name is used to grab the correct parameters.

```C++
class ExampleModel {
  VIPRA_MODULE_NAME("example_model")
};
```

#### VIPRA_REGISTER_PARAMS
#### VIPRA_PARAM

`VIPRA_REGISTER_PARAMS` provides the proper function signature for the parameter registration step.

`VIPRA_PARAM` registers a parameter in the parameter registration step.

When the parameters are loaded the variable is filled with the proper value.

```C++
class ExampleModel {
  public:
    VIPRA_REGISTER_PARAMS (
      VIPRA_PARAM("example_param", _example1),
      VIPRA_PARAM("another_param", _example2)
    )

  private:
   int _example1;
   std::string _example2;
}
```

## Model Macros

#### VIPRA_MODEL_INIT_STEP

`VIPRA_MODEL_INIT_STEP` provides the proper function signature for the model initialization step.

It provides the parameters:
- pedset: the pedestrian set module
- map: the map module
- goals: the goals module
- output: the output module
- engine: randomization engine

```C++
class ExampleModel {
  public:
    VIPRA_MODEL_INIT_STEP {
      // initialize model values
    }
}
```

#### VIPRA_MODEL_TIMESTEP

`VIPRA_MODEL_TIMESTEP` provides the proper function signature for the model timestep step.

It provides the parameters:
- pedset: the pedestrian set module
- map: the map module
- goals: the goals module
- output: the output module
- state: the [`State`] object to update
- deltaT: the size of a timestep
- timestep: the current timestep number

```C++
class ExampleModel {
  public:
    VIPRA_MODEL_TIMESTEP {
      // calculate pedestrian movement
      // update state parameter
    }
};
```

## Goals Macros

#### VIPRA_GOALS_INIT_STEP

`VIPRA_GOALS_INIT_STEP` provides the proper function signature for the goals initialization step.

It provides the parameters:
- pedset: the pedestrian set module
- map: the map module

```C++
class ExampleGoals {
  public:
    VIPRA_GOALS_INIT_STEP {
      // initialize pedestrian paths
    }
};
```