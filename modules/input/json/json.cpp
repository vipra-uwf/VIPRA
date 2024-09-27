
#include "json.hpp"
#include "vipra/macros/module.hpp"

namespace VIPRA::Input {

auto JSON::get_pedestrians() const -> std::optional<VIPRA::f3dVec>
{
  return {};
}

/**
 * @brief Returns polygons from the JSON file
 * @note Requires the JSON for the polygons to be in the format: {"key": [[{"x": 0, "y": 0}, {"x": 1, "y": 1}, ...], ...]} 
 * 
 * @param keys 
 * @return std::optional<std::vector<Geometry::Polygon>> 
 */
auto JSON::get_polygons(json_cref const& value)
    -> std::optional<std::vector<VIPRA::Geometry::Polygon>>
{
  std::vector<Geometry::Polygon> polygons;
  for ( auto const& polygon : value.get() ) {
    if ( ! polygon.is_array() ) return std::nullopt;

    std::vector<VIPRA::f3d> points;
    for ( auto const& point : polygon ) {
      if ( ! is_f2d(point) ) return std::nullopt;
      points.emplace_back(point.at("x").template get<VIPRA::f_pnt>(),
                          point.at("y").template get<VIPRA::f_pnt>(), 0);
    }

    polygons.emplace_back(points);
  }

  return polygons;
}

auto JSON::get_obstacles() const
    -> std::optional<std::vector<VIPRA::Geometry::Polygon>>
{
  return get_polygons(_json["obstacles"]);
}

auto JSON::get_spawns() const
    -> std::optional<std::vector<VIPRA::Geometry::Polygon>>
{
  return get_polygons(_json["spawns"]);
}

auto JSON::get_objectives() const
    -> std::optional<
        std::map<std::string, std::vector<VIPRA::Geometry::Polygon>>>
{
  if ( ! _json.contains("objectives") ) return std::nullopt;
  if ( ! _json["objectives"].is_array() ) return std::nullopt;

  std::map<std::string, std::vector<VIPRA::Geometry::Polygon>> objectives;

  for ( const auto& value : _json["objectives"] ) {
    // TODO(rolland): should this error? currently just skips any that don't match format
    if ( ! (value.contains("type") && value.contains("polygons")) ) continue;
    if ( ! (value["type"].is_string() && value["polygons"].is_array()) )
      continue;

    VIPRA::Log::debug("Adding Objectives of Type: {}",
                      value["type"].get<std::string>());

    auto objPolys = get_polygons(value["polygons"]);
    if ( ! objPolys ) continue;

    objectives[value["type"].get<std::string>()] = objPolys.value();

    VIPRA::Log::debug("Added {} Objectives of Type: {}",
                      objectives[value["type"].get<std::string>()].size(),
                      value["type"].get<std::string>());
  }

  return objectives;
}

auto JSON::get_areas() const
    -> std::optional<std::map<std::string, VIPRA::Geometry::Polygon>>
{
  // TODO(rolland): handle areas, currently just returns an empty map
  return std::map<std::string, VIPRA::Geometry::Polygon>{};
}

}  // namespace VIPRA::Input