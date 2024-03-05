#pragma once

#include <iostream>

#include "vipra/concepts/input.hpp"
#include "vipra/concepts/obstacle_set.hpp"

#include "vipra/geometry/polygon.hpp"
#include "vipra/modules.hpp"

#include "vipra/geometry/f3d.hpp"
#include "vipra/types/float.hpp"
#include "vipra/types/idx.hpp"
#include "vipra/types/parameter.hpp"
#include "vipra/types/size.hpp"
#include "vipra/util/debug_do.hpp"

// TODO(rolland): implement quadtree for storing obstacles

namespace VIPRA::Obstacles {
/**
 * @brief Obstacle module that uses a quadtree to store obstacles
 * 
 */
class QuadTree {
 public:
  VIPRA_MODULE_TYPE(OBSTACLES);

  void initialize(std::vector<VIPRA::Geometry::Polygon> const& obstacles,
                  std::vector<std::string> const&              types,
                  std::map<std::string, VIPRA::f3dVec> const&  objects) {
    _objectTypes = types;
    _objects = objects;
    _obstacles = obstacles;

    for (auto const& obstacle : obstacles) {
      for (auto const& edge : obstacle.edges) {
        _dimensions.x = std::max(_dimensions.x, edge.start.x);
        _dimensions.y = std::max(_dimensions.y, edge.start.y);
        _dimensions.z = std::max(_dimensions.z, edge.start.z);
        _dimensions.x = std::max(_dimensions.x, edge.end.x);
        _dimensions.y = std::max(_dimensions.y, edge.end.y);
        _dimensions.z = std::max(_dimensions.z, edge.end.z);
      }
    }
  }

  template <typename params_t>
  void register_params(params_t& params) {
    params.register_param(VIPRA::Modules::Type::OBSTACLES, "quad_tree", "minGridSize");
  }

  void config(auto& params, VIPRA::Random::Engine& engine) {
    _obsDistance = params.template get_param<VIPRA::f_pnt>(VIPRA::Modules::Type::OBSTACLES, "quad_tree",
                                                           "minGridSize", engine);
  }

  [[nodiscard]] auto get_dimensions() const -> VIPRA::f3d { return _dimensions; }

  [[nodiscard]] auto collision(VIPRA::f3d point) const -> bool {
    return std::any_of(_obstacles.begin(), _obstacles.end(),
                       [&](auto const& obstacle) { return obstacle.is_point_inside(point); });
  }

  [[nodiscard]] auto collision(VIPRA::Geometry::Circle circle) const -> bool {
    return std::any_of(_obstacles.begin(), _obstacles.end(),
                       [&](auto const& obstacle) { return obstacle.does_intersect(circle); });
  }

  [[nodiscard]] auto ray_hit(VIPRA::f3d start, VIPRA::f3d end) const -> VIPRA::f_pnt {
    VIPRA::f_pnt hit = std::numeric_limits<VIPRA::f_pnt>::max();
    for (auto const& obstacle : _obstacles) {
      for (auto const& edge : obstacle.edges) {
        if (!edge.does_intersect({start, end})) continue;
        auto intersection = edge.intersection_point({start, end});
        hit = std::min(hit, start.distance_to(intersection));
      }
    }
    return hit;
  }

  [[nodiscard]] auto get_object_types() const -> std::vector<std::string> const& { return _objectTypes; }

  [[nodiscard]] auto get_objects(std::string const& objType) const -> const VIPRA::f3dVec& {
    return _objects.at(objType);
  }

 private:
  std::vector<std::string>              _objectTypes;
  std::map<std::string, VIPRA::f3dVec>  _objects;
  std::vector<VIPRA::Geometry::Polygon> _obstacles;

  VIPRA::f_pnt _obsDistance;
  VIPRA::f3d   _dimensions;
};
}  // namespace VIPRA::Obstacles

CHECK_MODULE(ObstacleModule, VIPRA::Obstacles::QuadTree);