
#include "dxf.hpp"

#include "libdxfrw.h"

#include "vipra/macros/module.hpp"
#include "vipra/modules/map_input.hpp"

VIPRA_REGISTER_MODULE(DXF, MapInput)

/**
 * @brief Returns obstacles geometry from the .dxf file
 *
 * @return std::optional<std::vector<VIPRA::Geometry::Polygon>>
 */
auto DXF::get_obstacles() const
    -> std::optional<std::vector<VIPRA::Geometry::Polygon>>
{
  return {_obstacles};
}

/**
 * @brief Returns spawn area geometry from the .dxf file
 *
 * @return std::optional<std::vector<VIPRA::Geometry::Polygon>>
 */
auto DXF::get_spawns() const
    -> std::optional<std::vector<VIPRA::Geometry::Polygon>>
{
  return _spawns;
}

/**
 * @brief Returns Objectives map from the .dxf file
 *
 * @return std::optional<std::vector<VIPRA::Geometry::Polygon>>
 */
auto DXF::get_objectives() const
    -> std::optional<
        std::map<std::string, std::vector<VIPRA::Geometry::Polygon>>>
{
  return _objectives;
}

/**
 * @brief Returns spawn area geometry from the .dxf file
 *
 * @return std::optional<std::string, std::vector<VIPRA::Geometry::Polygon>>
 */
auto DXF::get_areas() const
    -> std::optional<std::map<std::string, VIPRA::Geometry::Polygon>>
{
  // TODO(tyler):
  std::map<std::string, VIPRA::Geometry::Polygon> areas;
  return areas;
}

/**
   * @brief Loads the DXF file from the given path
   *
   * @param filepath
   *
   * https://github.com/codelibs/libdxfrw
   */
void DXF::load(std::string const& filepath)
{
  VIPRA::Log::debug("DXF Loading {}", filepath);
  // Check Exists
  if ( ! std::filesystem::exists(filepath) )
    VIPRA_MODULE_ERROR("File does not exist at: {}", filepath);

  // Check if valid file
  if ( ! std::filesystem::is_regular_file(filepath) )
    VIPRA_MODULE_ERROR("{} is not a file", filepath)

  dxfRW dxfReader = dxfRW(filepath.c_str());

  VIPRA::Log::debug("DXF Reading {}", filepath);

  if ( ! dxfReader.read(&_drwReader, true) ) {
    VIPRA_MODULE_ERROR("Unable to parse DXF file at: {}", filepath);
  }

  // Get the items stored in reader and add to private variables.
  _objectives = _drwReader.get_objectives();
  _obstacles = _drwReader.get_obstacles();
  _spawns = _drwReader.get_pedestrians();
  _areas = _drwReader.get_areas();

  VIPRA::Log::debug("DXF Loaded");
}