#pragma once

// TODO(rolland): issue #15 implement quadtree for storing obstacles, currently just a vector

#include "vipra/macros/map.hpp"
#include "vipra/macros/module.hpp"
#include "vipra/macros/parameters.hpp"

#include "vipra/modules/map.hpp"
#include "vipra/modules/module.hpp"

namespace VIPRA::Map {
/**
 * @brief Map module that uses a quadtree to store obstacles
 * 
 */
class QuadTree : public VIPRA::Modules::Module<QuadTree>, public VIPRA::Modules::Map<QuadTree> {
 public:
  VIPRA_MODULE_NAME("QuadTree");
  VIPRA_MODULE_TYPE(Map);

  VIPRA_REGISTER_PARAMS(VIPRA_PARAM("minQuadSize", _obsDistance))

  VIPRA_MAP_INIT {}

 private:
  VIPRA::f_pnt _obsDistance{};
};
}  // namespace VIPRA::Map
