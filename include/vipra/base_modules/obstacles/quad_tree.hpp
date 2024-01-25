#pragma once

#include "vipra/concepts/input.hpp"
#include "vipra/concepts/obstacle_set.hpp"

#include "vipra/concepts/parameters.hpp"
#include "vipra/modules.hpp"

#include "vipra/types/f3d.hpp"
#include "vipra/types/float.hpp"
#include "vipra/types/idx.hpp"
#include "vipra/types/parameter.hpp"
#include "vipra/types/size.hpp"

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
    _obsDistance = params.template get_param<VIPRA::f_pnt>(VIPRA::Modules::Type::OBSTACLES, "minGridSize");
  }

  // TODO(rolland): implement
  [[nodiscard]] auto get_dimensions() const -> VIPRA::f3d { return _dimensions; }

  [[nodiscard]] auto collision(VIPRA::f3d point) const -> bool {
    return std::any_of(_obstacles.begin(), _obstacles.end(),
                       [&](const VIPRA::f3d& obs) { return obs.distance_to(point) < _obsDistance; });
  }

  [[nodiscard]] auto ray_hit(VIPRA::f3d start, VIPRA::f3d end) const -> VIPRA::f_pnt {
    if (start == end) {
      return -1;
    }

    VIPRA::f_pnt nearest = std::numeric_limits<VIPRA::f_pnt>::max();
    for (const auto& obstacle : _obstacles) {
      const VIPRA::f3d vAP = obstacle - start;
      const VIPRA::f3d vAB = end - start;

      const VIPRA::f_pnt sqDistanceAB = start.distance_to_sqrd(end);
      const VIPRA::f_pnt abapProduct = vAB.x * vAP.x + vAB.y * vAP.y;
      const VIPRA::f_pnt amount = abapProduct / sqDistanceAB;

      VIPRA::f3d intersect =
          VIPRA::f3d{(amount * (end.x - start.x)) + start.x, (amount * (end.y - start.y)) + start.y};

      if (intersect.distance_to_sqrd(obstacle) < _obsDistance * _obsDistance) {
        const VIPRA::f_pnt dist = intersect.distance_to(start);
        if (dist < nearest) {
          nearest = dist;
        }
      }
    }

    if (nearest == std::numeric_limits<VIPRA::f_pnt>::max()) {
      return -1;
    }
    return nearest;
  }

  [[nodiscard]] auto nearest_obstacle(VIPRA::f3d point) const -> VIPRA::f3d {
    const VIPRA::size obCnt = _obstacles.size();
    VIPRA::idx        closest = VIPRA::INVALID_IDX;
    VIPRA::f_pnt      cDist = std::numeric_limits<VIPRA::f_pnt>::max();

    for (VIPRA::idx i = 0; i < obCnt; ++i) {
      VIPRA::f_pnt dist = _obstacles[i].distance_to(point);
      if (dist < cDist) {
        cDist = dist;
        closest = i;
      }
    }
    return _obstacles[closest];
  }

  [[nodiscard]] auto nearest_obstacle_in_direction(VIPRA::f3d point, VIPRA::f3d direction) const
      -> VIPRA::f3d {
    const VIPRA::size obsCnt = _obstacles.size();
    VIPRA::idx        nearest = VIPRA::INVALID_IDX;
    VIPRA::f_pnt      cDist = std::numeric_limits<VIPRA::f_pnt>::max();

    for (VIPRA::idx i = 0; i < obsCnt; ++i) {
      if (direction_test(point, direction, _obstacles[i])) {
        VIPRA::f_pnt dist = _obstacles[i].distance_to_sqrd(point);
        if (dist < cDist) {
          cDist = dist;
          nearest = i;
        }
      }
    }

    if (nearest == VIPRA::INVALID_IDX) {
      return VIPRA::_emptyf3d_;
    }

    return _obstacles.at(nearest);
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

  static inline auto direction_test(VIPRA::f3d point, VIPRA::f3d direction, VIPRA::f3d obstacle) -> bool {
    const VIPRA::f3d displacement = point - obstacle;

    const VIPRA::f_pnt result =
        (displacement.x * direction.x) + (displacement.y * direction.y) + (displacement.z * direction.z);

    return result <= 0;
  }
};
}  // namespace VIPRA::Obstacles

CHECK_MODULE(ObstacleModule, VIPRA::Obstacles::QuadTree);