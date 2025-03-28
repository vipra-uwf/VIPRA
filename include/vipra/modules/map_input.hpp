#pragma once

#include <map>
#include <optional>
#include <vector>

#include "vipra/geometry/polygon.hpp"

namespace VIPRA::Modules {

/**
 * @brief Base MapInput Module
 * 
 */
class MapInput {
 public:
  virtual void load(std::string const&) = 0;

  /**
     * @brief Returns polygons as loaded from the input module, std::nullopt if incorrect type / doesn't exist
     *
     * @tparam data_t
     * @tparam key_ts
     * @param key
     * @return std::vector<Geometry::Polygon>
     */
  [[nodiscard]] virtual auto get_obstacles() const
      -> std::optional<std::vector<VIPRA::Geometry::Polygon>> = 0;

  /**
     * @brief Returns polygons as loaded from the input module, std::nullopt if incorrect type / doesn't exist
     *
     * @tparam data_t
     * @tparam key_ts
     * @param key
     * @return std::vector<Geometry::Polygon>
     */
  [[nodiscard]] virtual auto get_spawns() const
      -> std::optional<std::vector<VIPRA::Geometry::Polygon>> = 0;

  /**
     * @brief Returns polygons as loaded from the input module, std::nullopt if incorrect type / doesn't exist
     *
     * @tparam data_t
     * @tparam key_ts
     * @param key
     * @return std::vector<Geometry::Polygon>
     */
  [[nodiscard]] virtual auto get_objectives() const
      -> std::optional<std::map<std::string, std::vector<VIPRA::Geometry::Polygon>>> = 0;

  /**
     * @brief Returns polygons as loaded from the input module, std::nullopt if incorrect type / doesn't exist
     *
     * @tparam data_t
     * @tparam key_ts
     * @param key
     * @return std::vector<Geometry::Polygon>
     */
  [[nodiscard]] virtual auto get_areas() const
      -> std::optional<std::map<std::string, VIPRA::Geometry::Polygon>> = 0;

  virtual ~MapInput() = default;

  MapInput() = default;
  MapInput(const MapInput&) = default;
  MapInput(MapInput&&) = default;
  auto operator=(const MapInput&) -> MapInput& = default;
  auto operator=(MapInput&&) -> MapInput& = default;
};

}  // namespace VIPRA::Modules
