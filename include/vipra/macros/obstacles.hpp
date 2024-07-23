#pragma once

#define VIPRA_OBSTACLES_INIT                                             \
  void init_step(std::vector<VIPRA::Geometry::Polygon> const& obstacles, \
                 std::vector<std::string> const& types, std::map<std::string, VIPRA::f3dVec> const& objects)
