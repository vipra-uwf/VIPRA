---
title: Goals Module Implementation
layout: template
filename: goals.md
--- 

# Goals Implementation

`Goals` modules handle pathfinding for the simulation.

<!-- TODO: move relavent macros here -->

```C++
void initialize(PedSetModule auto const&, MapModule auto const&);
void update(PedSetModule auto const&, MapModule auto const&, VIPRA::delta_t);
void change_end_goal(VIPRA::idx, VIPRA::f3d);

auto current_goals() -> std::vector<VIPRA::f3d> const&;
auto end_goals() -> std::vector<VIPRA::f3d> const&;
auto current_goal(VIPRA::idx) -> VIPRA::f3d const&;
auto end_goal(VIPRA::idx) -> VIPRA::f3d const&;
auto is_goal_met(VIPRA::idx) -> bool;
auto is_sim_goal_met() -> bool;
auto time_since_last_goal(VIPRA::idx) -> VIPRA::f_pnt;
```
