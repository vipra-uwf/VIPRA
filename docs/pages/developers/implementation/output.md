
# Output Implementation

# Required Methods

<!-- TODO: move relavent macros here -->

```C++
virtual void write(std::filesystem::path const& outputDir) = 0;
virtual void timestep_update(VIPRA::timestep, VIPRA::delta_t,
                              VIPRA::State const&) = 0;
```


---


