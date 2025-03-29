#pragma once

#define VIPRA_MAP_INIT                                                                \
  void init_step(                                                                     \
      std::vector<VIPRA::Geometry::Polygon> const&                        obstacles,  \
      std::map<std::string, std::vector<VIPRA::Geometry::Polygon>> const& objectives, \
      std::vector<VIPRA::Geometry::Polygon> const&                        spawns,     \
      std::map<std::string, VIPRA::Geometry::Polygon> const&              areas)

#define VIPRA_MAP_RESET void reset_map_module()