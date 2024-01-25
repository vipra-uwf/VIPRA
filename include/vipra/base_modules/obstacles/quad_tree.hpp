#pragma once

#include "vipra/concepts/input.hpp"
#include "vipra/concepts/obstacle_set.hpp"

#include "vipra/concepts/parameters.hpp"
#include "vipra/modules.hpp"

#include "vipra/types/f3d.hpp"
#include "vipra/types/float.hpp"
#include "vipra/types/parameter.hpp"

// TODO(rolland): implement quadtree for storing obstacles

namespace VIPRA::Obstacles {
class QuadTree {
  VIPRA_MODULE_TYPE(OBSTACLES);

 public:
  void initialize(const std::vector<VIPRA::f3d>& obstacles, const std::vector<std::string>& types,
                  const std::map<std::string, VIPRA::f3dVec>& objects) {
    _obstacles = obstacles;
    _objectTypes = types;
    _objects = objects;

    for (const auto& obstacle : _obstacles) {
      _dimensions.x = std::max(_dimensions.x, obstacle.x);
      _dimensions.y = std::max(_dimensions.y, obstacle.y);
      _dimensions.z = std::max(_dimensions.z, obstacle.z);
    }
  }

  template <Concepts::ParamModule params_t>
  static void register_params() {
    params_t::register_param(VIPRA::Modules::Type::OBSTACLES, "minGridSize", VIPRA::ParameterType::REQUIRED);
  }

  void setup(auto& params) {
    params.template register_param(VIPRA::Modules::Type::OBSTACLES, "minGridSize",
                                   VIPRA::ParameterType::REQUIRED);
  }

  // TODO(rolland): implement
  [[nodiscard]] auto get_dimensions() const -> VIPRA::f3d { return _dimensions; }
  [[nodiscard]] auto collision(VIPRA::f3d) const -> bool { return false; }
  [[nodiscard]] auto ray_hit(VIPRA::f3d, VIPRA::f3d) const -> VIPRA::f_pnt { return 1.0F; }
  [[nodiscard]] auto nearest_obstacle(VIPRA::f3d) const -> VIPRA::f3d { return VIPRA::f3d{}; }
  [[nodiscard]] auto nearest_obstacle_in_direction(VIPRA::f3d, VIPRA::f3d) const -> VIPRA::f3d {
    return VIPRA::f3d{};
  }
  [[nodiscard]] auto get_object_types() const -> const std::vector<std::string>& { return _objectTypes; }
  [[nodiscard]] auto get_objects(const std::string& objType) const -> const VIPRA::f3dVec& {
    return _objects.at(objType);
  }

 private:
  std::vector<std::string>             _objectTypes;
  std::map<std::string, VIPRA::f3dVec> _objects;
  std::vector<VIPRA::f3d>              _obstacles;

  VIPRA::f_pnt _obsDistance;
  VIPRA::f3d   _dimensions;
};
}  // namespace VIPRA::Obstacles

CHECK_MODULE(ObstacleModule, VIPRA::Obstacles::QuadTree);