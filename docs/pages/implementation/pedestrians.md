---
title: Pedestrian Module Implementation
layout: template
filename: pedestrians.md
--- 

# Pedestrian Set Implementation

### Interface

<!-- TODO: move relavent macros here -->

```C++
auto num_pedestrians() -> VIPRA::size;

auto ped_coords(VIPRA::idx) const -> VIPRA::f3d;
auto all_coords() const -> std::vector<VIPRA::f3d> const&;
auto ped_velocity(VIPRA::idx) const -> VIPRA::f3d;
auto all_velocities() const -> std::vector<VIPRA::f3d> const&;
auto closest_ped() const -> VIPRA::idx;

void update(VIPRA::State const&);
```