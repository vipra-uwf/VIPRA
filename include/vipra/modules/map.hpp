#pragma once

#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include "vipra/geometry/f3d.hpp"
#include "vipra/geometry/polygon.hpp"

#include "vipra/logging/logging.hpp"
#include "vipra/types/float.hpp"
#include "vipra/util/crtp.hpp"

namespace VIPRA::Modules {
/**
 * @brief Base Map Module Class
 * 
 */
template <typename module_t>
class Map : public Util::CRTP<Map<module_t>> {
  using Util::CRTP<Map<module_t>>::self;

  // NOLINTBEGIN
 public:
  void initialize(auto&& input)
  {
    auto const obstacles = input.template get_obstacles();
    if ( ! obstacles ) throw std::runtime_error("Input Module failed to load map obstacles");

    auto const objectives = input.template get_objectives();
    if ( ! objectives ) throw std::runtime_error("Input Module failed to load map objectives");

    auto const spawns = input.template get_spawns();
    if ( ! spawns ) throw std::runtime_error("Input Module failed to load map pedestrians spawn areas");

    auto const areas = input.template get_areas();
    if ( ! areas ) throw std::runtime_error("Input Module failed to load map areas");

    _obstacles = std::move(obstacles.value());
    _objectives = std::move(objectives.value());
    _spawns = std::move(spawns.value());
    _areas = std::move(areas.value());

    for ( auto const& [type, _] : _objectives ) {
      _objectiveTypes.push_back(type);
    }

    find_dimensions(_obstacles, _objectives, _spawns, _areas);

    // initialize derived obstacles module
    return self().init_step(_obstacles, _objectives, _spawns, _areas);
  }

  [[nodiscard]] auto get_dimensions() const -> VIPRA::f3d { return _dimensions; }

  [[nodiscard]] auto collision(VIPRA::f3d point) const -> bool
  {
    return std::any_of(_obstacles.begin(), _obstacles.end(),
                       [&](auto const& obstacle) { return obstacle.is_point_inside(point); });
  }

  [[nodiscard]] auto collision(VIPRA::Geometry::Circle radius) const -> bool
  {
    return std::any_of(_obstacles.begin(), _obstacles.end(),
                       [&](auto const& obstacle) { return obstacle.does_intersect(radius); });
  }

  [[nodiscard]] auto ray_hit(VIPRA::f3d start, VIPRA::f3d end) const -> VIPRA::f_pnt
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

  [[nodiscard]] auto get_objective_types() const -> std::vector<std::string> const&
  {
    return _objectiveTypes;
  }

  [[nodiscard]] auto get_objectives(std::string const& type) const
      -> std::vector<VIPRA::Geometry::Polygon> const&
  {
    auto iter = _objectives.find(type);
    if ( iter == _objectives.end() ) {
      throw std::runtime_error("Map does NOT have objectives of type " + type);
    }

    return iter->second;
  }

  [[nodiscard]] auto get_obstacles() const -> std::vector<VIPRA::Geometry::Polygon> const&
  {
    return _obstacles;
  }

  [[nodiscard]] auto get_spawns() const -> std::vector<VIPRA::Geometry::Polygon> const& { return _spawns; }

  [[nodiscard]] auto get_objectives() const
      -> std::map<std::string, std::vector<VIPRA::Geometry::Polygon>> const&
  {
    return _objectives;
  }

  [[nodiscard]] auto get_areas() const -> std::map<std::string, VIPRA::Geometry::Polygon> { return _areas; }

 private:
  std::vector<VIPRA::Geometry::Polygon>                        _obstacles;
  std::map<std::string, std::vector<VIPRA::Geometry::Polygon>> _objectives;
  std::vector<VIPRA::Geometry::Polygon>                        _spawns;
  std::map<std::string, VIPRA::Geometry::Polygon>              _areas;

  std::vector<std::string> _objectiveTypes;

  VIPRA::f3d _dimensions;

  void find_dimensions(std::vector<VIPRA::Geometry::Polygon> const&                        obstacles,
                       std::map<std::string, std::vector<VIPRA::Geometry::Polygon>> const& objectives,
                       std::vector<VIPRA::Geometry::Polygon> const&                        spawns,
                       std::map<std::string, VIPRA::Geometry::Polygon> const&              areas)
  {
    auto setToMax = [&](auto const& polygon) {
      for ( auto const& edge : polygon.edges ) {
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

    VIPRA::Log::debug("Map Dimensions: {}, {}, {}", _dimensions.x, _dimensions.y, _dimensions.z);
  }
};
}  // namespace VIPRA::Modules