
# Module Implementation

Here are the requirements for each of the module types.

1. [Base Module](./implementation/module.md) - Requirements common to all module types

2. [Model](./implementation/model.md)

3. [Goals](./implementation/goals.md)

4. [Maps](./implementation/map.md)

5. [Pedestrians](./implementation/pedestrians.md)

6. [Input](./implementation/input.md)

7. [Output](./implementation/output.md)


# Module Views

VIPRA Modules are passed to methods as template parameters; This means that methods that require access to those modules need to be defined in a header file.

Module Views allow for defining methods in a .cpp file; this reduces compilation time and is mainly meant as a tool to help developers iterate on their module implementations faster.

Using Views reduces performance, so it is recommended that completed modules switch to taking modules as templates where possible.

An example module using Views is found under the `examples/views_model` directory.

Example:
```C++
// Method that needs to use the pedestrian set to get the positions of a pedestrian

// Template Implementation
template <typename pedset_t>
void template_example(pedset_t const& pedestrians) {
  VIPRA::f3d pedPos = pedestrians.ped_coords(5);
  // ... use the position
}

// --------------------------------------------

// Views Implementation

// .hpp file
void template_example(VIPRA::Views::Pedestrians pedestrians);

// .cpp file
void template_example(VIPRA::Views::Pedestrians pedestrians) {
  VIPRA::f3d pedPos = pedestrians.ped_coords(5);
  // ... use the position
}
```