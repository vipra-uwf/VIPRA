
# Usage

# Choosing Module Implementations

In `main.cpp` you will find this line:
```C++
auto sim = VIPRA::simulation(CALM::Model{}, 
                             VIPRA::Goals::AStar{}, 
                             VIPRA::Pedestrians::Grid{},
                             VIPRA::CoordModules::Output{
                              VIPRA::Output::Trajectories::JSON{}
                             },
                             VIPRA::Obstacles::QuadTree{});
```

`VIPRA::simulation` is the method used to create a simulation

Each simulation requires one of each of the [Module Types](../modules.md):
1. Pedestrian Dynamics Model
2. Goals
3. Pedestrian Set
4. Obstacle Set
5. Output Coordinator (with any number of Input modules)

**To change what modules to use** simply switch out the module type here and re-run `make`

### Example

Assume we have the pedestrian dynamics model we created in the [Model Implementation Example](../examples/model_example.md)

To use this model, rather than the CALM model we would switch the above line to:
```C++
auto sim = VIPRA::simulation(ExampleModel{}, 
                             VIPRA::Goals::AStar{}, 
                             VIPRA::Pedestrians::Grid{},
                             VIPRA::CoordModules::Output{
                              VIPRA::Output::Trajectories::JSON{}
                             },
                             VIPRA::Obstacles::QuadTree{});
```

We can then run `make` and our simulation will now use the `ExampleModel`