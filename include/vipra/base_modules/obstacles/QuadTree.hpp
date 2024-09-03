#pragma once

// TODO(rolland): issue #15 implement quadtree for storing obstacles, currently just a vector

#include <map>
#include <string>
#include <vector>

#include "vipra/macros/module.hpp"
#include "vipra/macros/obstacles.hpp"

#include "vipra/macros/parameters.hpp"
#include "vipra/macros/performance.hpp"
#include "vipra/modules/obstacles.hpp"

namespace VIPRA::Obstacles {
/**
 * @brief Obstacle module that uses a quadtree to store obstacles
 * 
 */
class QuadTree : public VIPRA::Modules::Module<QuadTree>, public VIPRA::Modules::Obstacles<QuadTree> {
 public:
  VIPRA_MODULE_NAME("QuadTree");
  VIPRA_MODULE_TYPE(Obstacles);

  VIPRA_REGISTER_PARAMS(VIPRA_PARAM("minQuadSize", _obsDistance))

  VIPRA_OBSTACLES_INIT
  {
    _objectTypes = types;
    _objects = objects;
    _obstacles = obstacles;

    // Get dimensions of the map
    for ( auto const& obstacle : obstacles ) {
      for ( auto const& edge : obstacle.edges ) {
        _dimensions.x = std::max(_dimensions.x, edge.start.x);
        _dimensions.y = std::max(_dimensions.y, edge.start.y);
        _dimensions.z = std::max(_dimensions.z, edge.start.z);
        _dimensions.x = std::max(_dimensions.x, edge.end.x);
        _dimensions.y = std::max(_dimensions.y, edge.end.y);
        _dimensions.z = std::max(_dimensions.z, edge.end.z);
      }
    }
  }

  [[nodiscard]] auto get_dimensions_impl() const -> VIPRA::f3d { return _dimensions; }

  [[nodiscard]] auto collision_impl(VIPRA::f3d point) const -> bool
  {
    return std::any_of(_obstacles.begin(), _obstacles.end(),
                       [&](auto const& obstacle) { return obstacle.is_point_inside(point); });
  }

  [[nodiscard]] auto collision_impl(VIPRA::Geometry::Circle circle) const -> bool
  {
    return std::any_of(_obstacles.begin(), _obstacles.end(),
                       [&](auto const& obstacle) { return obstacle.does_intersect(circle); });
  }

  [[nodiscard]] auto ray_hit_impl(VIPRA::f3d start, VIPRA::f3d end) const -> VIPRA::f_pnt
  {
    VIPRA::f_pnt hit = std::numeric_limits<VIPRA::f_pnt>::max();
    for ( auto const& obstacle : _obstacles ) {
      for ( auto const& edge : obstacle.edges ) {
        if ( ! edge.does_intersect({start, end}) ) continue;
        auto intersection = edge.intersection_point({start, end});
        hit = std::min(hit, start.distance_to(intersection));
      }
    }
    return hit;
  }

  [[nodiscard]] auto get_object_types_impl() const -> std::vector<std::string> const& { return _objectTypes; }

  [[nodiscard]] auto get_objects_impl(std::string const& objType) const -> const VIPRA::f3dVec&
  {
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
