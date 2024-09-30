#pragma once
#include "vipra/macros/parameters.hpp"
#define _USE_MATH_DEFINES

#include <cmath>
#include <filesystem>
#include <optional>
#include <string>

#include "vipra/geometry/polygon.hpp"
#include "vipra/macros/module.hpp"
#include "vipra/modules/map_input.hpp"
#include "vipra/modules/serializable.hpp"

#include "drw_reader.hpp"

/* ERRORS:
0   BAD_NONE
1   BAD_UNKNOWN
2   BAD_OPEN
3   BAD_VERSION
4   BAD_READ_METADATA
5   BAD_READ_FILE_HEADER
6   BAD_READ_HEADER
7   BAD_READ_HANDLES
8   BAD_READ_CLASSES
9   BAD_READ_TABLES
10  BAD_READ_BLOCKS
11  BAD_READ_ENTITIES
12  BAD_READ_OBJECTS
13  BAD_READ_SECTION
14  BAD_CODE_PARSED
*/

/**
  * @brief Parameter and Polygon qualified dxf input module
  *
  *
  */
class DXF : public VIPRA::Modules::Module<DXF>,
            public VIPRA::Modules::Serializable,
            public VIPRA::Modules::MapInput {
 public:
  VIPRA_MODULE_NAME("DXF");
  VIPRA_MODULE_TYPE(MapInput);

  VIPRA_REGISTER_PARAMS()

  void load(std::string const& filepath) override;

  // TODO(rolland): implement
  [[nodiscard]] auto serialize() -> std::string override { return ""; }
  void               parse(std::string const& /*data*/) override {}

  [[nodiscard]] auto get_obstacles() const
      -> std::optional<std::vector<VIPRA::Geometry::Polygon>> override;
  [[nodiscard]] auto get_spawns() const
      -> std::optional<std::vector<VIPRA::Geometry::Polygon>> override;
  [[nodiscard]] auto get_objectives() const
      -> std::optional<std::map<
          std::string, std::vector<VIPRA::Geometry::Polygon>>> override;
  [[nodiscard]] auto get_areas() const
      -> std::optional<
          std::map<std::string, VIPRA::Geometry::Polygon>> override;

  [[nodiscard]] auto get_objective_types(std::vector<std::string> const& keys)
      const -> std::optional<std::vector<std::string>>
  {
    if ( keys.size() != 1 ) return std::nullopt;
    if ( keys[0] != "obj_types" ) return std::nullopt;

    std::vector<std::string> types;
    types.reserve(_objectives.size());

    for ( auto const& [type, coords] : _objectives ) {
      types.push_back(type);
    }

    return {types};
  }

  [[nodiscard]] static auto get_polygons(
      std::vector<std::shared_ptr<DRW_Vertex>>& vertexList)
      -> std::optional<std::vector<VIPRA::Geometry::Polygon>>;

 private:
  std::filesystem::path _filepath;

  std::map<std::string, std::vector<VIPRA::Geometry::Polygon>> _objectives;
  std::vector<VIPRA::Geometry::Polygon>                        _obstacles;
  std::vector<VIPRA::Geometry::Polygon>                        _spawns;

  std::map<std::string, VIPRA::Geometry::Polygon> _areas;

  VIPRA::DrwReader _drwReader;
};
