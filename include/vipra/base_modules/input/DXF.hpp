#pragma once
#define _USE_MATH_DEFINES

#include <cmath>
#include <filesystem>
#include <optional>
#include <string>

#include <libdxfrw.h>

#include "vipra/geometry/polygon.hpp"
#include "vipra/macros/module.hpp"
#include "vipra/modules/input.hpp"
#include "vipra/modules/map_input.hpp"
#include "vipra/modules/serializable.hpp"

#include "dxf/drw_reader.hpp"

namespace VIPRA::Input {
/**
  * @brief Parameter and Polygon qualified dxf input module
  *
  *
  */
class DXF : public VIPRA::Modules::Module<DXF>,
            public VIPRA::Modules::Input<DXF>,
            public VIPRA::Modules::Serializable<DXF>,
            public VIPRA::Modules::MapInput<DXF> {
 public:
  VIPRA_MODULE_NAME("DXF");
  VIPRA_MODULE_TYPE(Input);

  explicit DXF(std::filesystem::path filepath) : _filepath(std::move(filepath)) {}

  void load_impl();

  template <typename data_t>
  [[nodiscard]] auto get_impl(std::vector<std::string> const& keys) const -> std::optional<data_t>
  {
    return std::nullopt;
  }

  // TODO(rolland): implement
  [[nodiscard]] auto to_string() const -> std::string { return ""; }
  void               parse_impl(std::string const&) { return; }

  [[nodiscard]] auto get_obstacles_impl() const -> std::optional<std::vector<VIPRA::Geometry::Polygon>>;
  [[nodiscard]] auto get_spawns_impl() const -> std::optional<std::vector<VIPRA::Geometry::Polygon>>;
  [[nodiscard]] auto get_objectives_impl() const
      -> std::optional<std::map<std::string, std::vector<VIPRA::Geometry::Polygon>>>;
  [[nodiscard]] auto get_areas_impl() const -> std::optional<std::map<std::string, VIPRA::Geometry::Polygon>>;

  [[nodiscard]] auto get_objective_types(std::vector<std::string> const& keys) const
      -> std::optional<std::vector<std::string>>
  {
    if ( keys.size() != 1 ) return std::nullopt;
    if ( keys[0] != "obj_types" ) return std::nullopt;

    std::vector<std::string> types;
    for ( auto const& [type, coords] : _objectives ) {
      types.push_back(type);
    }

    return {types};
  }

  [[nodiscard]] static auto get_polygons(std::vector<std::shared_ptr<DRW_Vertex>>& vertexList)
      -> std::optional<std::vector<VIPRA::Geometry::Polygon>>;

 private:
  std::filesystem::path _filepath;

  std::map<std::string, std::vector<VIPRA::Geometry::Polygon>> _objectives;
  std::vector<VIPRA::Geometry::Polygon>                        _obstacles;
  std::vector<VIPRA::Geometry::Polygon>                        _spawns;

  std::map<std::string, VIPRA::Geometry::Polygon> _areas;

  DrwReader drw_reader;
};
}  // namespace VIPRA::Input

/**
 * @brief Returns polygons from the DXF file
 *
 * @param keys
 * @return std::optional<std::vector<VIPRA::Geometry::Polygon>>
 */
template <>
inline auto VIPRA::Input::DXF::get_impl<std::vector<VIPRA::Geometry::Polygon>>(
    std::vector<std::string> const& keys) const -> std::optional<std::vector<VIPRA::Geometry::Polygon>>
{
  return drw_reader.get_vipra_polygons();
}

/**
 * @brief Returns obstacles geometry from the .dxf file
 *
 * @return std::optional<std::vector<VIPRA::Geometry::Polygon>>
 */
inline auto VIPRA::Input::DXF::get_obstacles_impl() const
    -> std::optional<std::vector<VIPRA::Geometry::Polygon>>
{
  return {_obstacles};
}

/**
 * @brief Returns spawn area geometry from the .dxf file
 *
 * @return std::optional<std::vector<VIPRA::Geometry::Polygon>>
 */
inline auto VIPRA::Input::DXF::get_spawns_impl() const -> std::optional<std::vector<VIPRA::Geometry::Polygon>>
{
  return std::nullopt;
}

/**
 * @brief Returns Objectives map from the .dxf file
 *
 * @return std::optional<std::vector<VIPRA::Geometry::Polygon>>
 */
inline auto VIPRA::Input::DXF::get_objectives_impl() const
    -> std::optional<std::map<std::string, std::vector<VIPRA::Geometry::Polygon>>>
{
  return _objectives;
}

/**
 * @brief Returns spawn area geometry from the .dxf file
 *
 * @return std::optional<std::vector<VIPRA::Geometry::Polygon>>
 */
inline auto VIPRA::Input::DXF::get_areas_impl() const
    -> std::optional<std::map<std::string, VIPRA::Geometry::Polygon>>
{
  return std::nullopt;
}

/**
   * @brief Loads the DXF file from the given path
   *
   * @param filepath
   *
   * https://github.com/codelibs/libdxfrw
   */
inline void VIPRA::Input::DXF::load_impl()
{
  // Check Exists
  if ( ! std::filesystem::exists(_filepath) )
    throw std::runtime_error("File does not exist at: " + _filepath.string());

  // Check if valid file
  if ( ! std::filesystem::is_regular_file(_filepath) )
    throw std::runtime_error("File is not a regular file at: " + _filepath.string());

  const char* fileCharArr = _filepath.string().c_str();
  dxfRW       dxfReader = dxfRW(fileCharArr);

  dxfReader.read(&drw_reader, 1);

  // Get the items stored in reader and add to private variables.
  _objectives = drw_reader.getObjectives();
  _obstacles = drw_reader.getObstacles();
  _spawns = drw_reader.getPedestrians();
  _areas = drw_reader.getAreas();
}
