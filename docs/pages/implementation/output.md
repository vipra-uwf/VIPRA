---
title: Output Module Implementation
layout: template
filename: output.md
--- 

# Output Implementation

### Interface

<!-- TODO: move relavent macros here -->

```C++
auto write();

template <typename value_t>
void timestep_value(std::string const&, VIPRA::timestep, value_t);

template <typename value_t>
void ped_timestep_value(VIPRA::idx, VIPRA::timestep, std::string const&, value_t);
```
---
