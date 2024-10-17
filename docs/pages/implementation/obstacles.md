---
title: Obstacles Module Implementation
layout: template
filename: obstacles.md
--- 

# Obstacle Set Implementation

Obstacle Set modules hold the geometry of the environment and handle map/pedestrian collision detection.

<!-- TODO: move relavent macros here -->

```C++
void initialize(std::vector<VIPRA::Geometry::Polygon> const&, std::vector<std::string> const&, std::map<std::string, std::vector<VIPRA::f3d>> const&);

auto get_object_types() -> std::vector<std::string> const&;
auto get_objects(std::string const&) -> std::vector<VIPRA::f3d> const&;

auto collision(VIPRA:f3d) -> bool;
auto collision(VIPRA::Geometry::Circle) -> bool;
auto ray_hit(VIPRA::f3d, VIPRA::f3d) -> VIPRA::f_pnt;

auto get_dimensions() -> VIPRA::f3d;
```