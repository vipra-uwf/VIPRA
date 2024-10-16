#pragma once

#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include "vipra/geometry/geometry.hpp"

#include "vipra/geometry/polygon.hpp"
#include "vipra/logging/logging.hpp"
#include "vipra/macros/module.hpp"
#include "vipra/modules/map_input.hpp"
#include "vipra/types/float.hpp"

#include "vipra/macros/map.hpp"

namespace VIPRA::Modules {
/**
 * @brief Base Map Module Class
 * 
 */
class Map {
 public:
  // FORWARD_REGISTER_PARAMS;

  virtual VIPRA_MAP_INIT = 0;

  virtual void obstacle_added(VIPRA::Geometry::Polygon const& obstacle) {}
  virtual void objective_added(std::string const&              type,
                               VIPRA::Geometry::Polygon const& objective)
  {
  }
  virtual void area_added(std::string const&              name,
                          VIPRA::Geometry::Polygon const& polygon)
  {
  }

  [[nodiscard]] virtual auto collision(VIPRA::f3d point) const -> bool
  {
    return std::any_of(
        _obstacles.begin(), _obstacles.end(),
        [&](auto const& obstacle) { return obstacle.is_point_inside(point); });
  }
  [[nodiscard]] virtual auto collision(
      VIPRA::Geometry::Circle const& radius) const -> bool
  {
    if ( collision(radius.center()) ) return true;

    return std::any_of(_obstacles.begin(), _obstacles.end(),
                       [&](Geometry::Polygon const& obstacle) {
                         return Geometry::do_intersect(obstacle, radius);
                       });
  }

  [[nodiscard]] virtual auto ray_hit(VIPRA::f3d start,
                                     VIPRA::f3d end) const -> VIPRA::f_pnt
  {
    VIPRA::f_pnt hit = std::numeric_limits<VIPRA::f_pnt>::max();
    for ( auto const& obstacle : _obstacles ) {
      for ( auto const& edge : obstacle.sides() ) {
        if ( ! edge.does_intersect({start, end}) ) continue;
        auto intersection = edge.intersection_point({start, end});
        hit = std::min(hit, start.distance_to(intersection));
      }
    }
    return hit;
  }

  void add_obstacle(VIPRA::Geometry::Polygon const& obstacle)
  {
    _obstacles.push_back(obstacle);

    if ( _initialized ) obstacle_added(obstacle);
  }
  void add_objective(std::string const&              type,
                     VIPRA::Geometry::Polygon const& objective)
  {
    if ( ! _objectives.contains(type) ) _objectives[type] = {};

    _objectives[type].push_back(objective);

    if ( _initialized ) objective_added(type, objective);
  }
  void add_spawn(VIPRA::Geometry::Polygon const& spawn)
  {
    _spawns.push_back(spawn);
  }
  void add_area(std::string const&              name,
                VIPRA::Geometry::Polygon const& polygon)
  {
    _areas[name] = polygon;

    if ( _initialized ) area_added(name, polygon);
  }

  [[nodiscard]] auto get_objectives() const
      -> std::map<std::string, std::vector<VIPRA::Geometry::Polygon>> const&
  {
    return _objectives;
  }
  [[nodiscard]] auto get_objectives(std::string const& type) const
      -> std::vector<VIPRA::Geometry::Polygon> const&
  {
    auto iter = _objectives.find(type);
    if ( iter == _objectives.end() ) {
      VIPRA_BASE_MODULE_ERROR("Map", "Map does NOT have objectives of type: {}",
                              type);
    }

    return iter->second;
  }

  [[nodiscard]] auto get_dimensions() const -> VIPRA::f3d
  {
    return _dimensions;
  }
  [[nodiscard]] auto get_objective_types() const
      -> std::vector<std::string> const&
  {
    return _objectiveTypes;
  }
  [[nodiscard]] auto get_obstacles() const
      -> std::vector<VIPRA::Geometry::Polygon> const&
  {
    return _obstacles;
  }
  [[nodiscard]] auto get_spawns() const
      -> std::vector<VIPRA::Geometry::Polygon> const&
  {
    return _spawns;
  }
  [[nodiscard]] auto get_areas() const
      -> std::map<std::string, VIPRA::Geometry::Polygon>
  {
    return _areas;
  }

  void initialize(VIPRA::Modules::MapInput& input)
  {
    VIPRA::Log::debug("Initializing Map");

    auto obstacles = input.get_obstacles();
    if ( ! obstacles )
      VIPRA_BASE_MODULE_ERROR("Map",
                              "Input Module failed to load map obstacles");

    auto objectives = input.get_objectives();
    if ( ! objectives )
      VIPRA_BASE_MODULE_ERROR("Map",
                              "Input Module failed to load map objectives");

    auto spawns = input.get_spawns();
    if ( ! spawns )
      VIPRA_BASE_MODULE_ERROR(
          "Map", "Input Module failed to load map pedestrians spawn areas");

    auto areas = input.get_areas();
    if ( ! areas )
      VIPRA_BASE_MODULE_ERROR("Map", "Input Module failed to load map areas");

    _obstacles = std::move(obstacles.value());
    _objectives = std::move(objectives.value());
    _spawns = std::move(spawns.value());
    _areas = std::move(areas.value());

    for ( auto const& [type, _] : _objectives ) {
      _objectiveTypes.push_back(type);
    }

    find_dimensions(_obstacles, _objectives, _spawns, _areas);

    // initialize derived obstacles module
    init_step(_obstacles, _objectives, _spawns, _areas);

    VIPRA::Log::debug("Map Initialized");
  }

 private:
  std::vector<VIPRA::Geometry::Polygon>                        _obstacles;
  std::map<std::string, std::vector<VIPRA::Geometry::Polygon>> _objectives;
  std::vector<VIPRA::Geometry::Polygon>                        _spawns;
  std::map<std::string, VIPRA::Geometry::Polygon>              _areas;

  std::vector<std::string> _objectiveTypes;

  bool       _initialized{false};
  VIPRA::f3d _dimensions;

  void find_dimensions(
      std::vector<VIPRA::Geometry::Polygon> const& obstacles,
      std::map<std::string, std::vector<VIPRA::Geometry::Polygon>> const&
                                                             objectives,
      std::vector<VIPRA::Geometry::Polygon> const&           spawns,
      std::map<std::string, VIPRA::Geometry::Polygon> const& areas)
  {
    auto setToMax = [&](auto const& polygon) {
      for ( auto const& edge : polygon.sides() ) {
        _dimensions.x = std::max({_dimensions.x, edge.start.x, edge.end.x});
        _dimensions.y = std::max({_dimensions.y, edge.start.y, edge.end.y});
        _dimensions.z = std::max({_dimensions.z, edge.start.z, edge.end.z});
      }
    };

    auto setToAllMax = [&](auto const& polygons) {
      for ( auto const& polygon : polygons ) {
        setToMax(polygon);
      }
    };

    setToAllMax(obstacles);
    setToAllMax(spawns);

    for ( auto const& [_, polygons] : objectives ) {
      setToAllMax(polygons);
    };

    for ( auto const& [_, polygon] : areas ) {
      setToMax(polygon);
    };

    VIPRA::Log::debug("Map Dimensions: {}, {}, {}", _dimensions.x,
                      _dimensions.y, _dimensions.z);
  }

 public:
  virtual ~Map() = default;
  Map() = default;
  Map(const Map&) = default;
  Map(Map&&) noexcept = default;
  auto operator=(const Map&) -> Map& = default;
  auto operator=(Map&&) noexcept -> Map& = default;
};
}  // namespace VIPRA::Modules