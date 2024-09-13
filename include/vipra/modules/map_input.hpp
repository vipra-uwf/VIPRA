#pragma once

#include <map>
#include <optional>
#include <vector>

#include "vipra/geometry/polygon.hpp"
#include "vipra/modules/util.hpp"
#include "vipra/util/crtp.hpp"

namespace VIPRA::Modules {

/**
 * @brief Base MapInput Module
 * 
 */
template <typename module_t>
class MapInput : public Util::CRTP<MapInput<module_t>> {
  using Util::CRTP<MapInput<module_t>>::self;

 public:
  FORWARD_REGISTER_PARAMS;

  /**
     * @brief Returns polygons as loaded from the input module, std::nullopt if incorrect type / doesn't exist
     *
     * @tparam data_t
     * @tparam key_ts
     * @param key
     * @return std::vector<Geometry::Polygon>
     */
  [[nodiscard]] auto get_obstacles() const -> std::optional<std::vector<VIPRA::Geometry::Polygon>>
  {
    return self().template get_obstacles_impl();
  }

  /**
     * @brief Returns polygons as loaded from the input module, std::nullopt if incorrect type / doesn't exist
     *
     * @tparam data_t
     * @tparam key_ts
     * @param key
     * @return std::vector<Geometry::Polygon>
     */
  [[nodiscard]] auto get_spawns() const -> std::optional<std::vector<VIPRA::Geometry::Polygon>>
  {
    return self().template get_spawns_impl();
  }

  /**
     * @brief Returns polygons as loaded from the input module, std::nullopt if incorrect type / doesn't exist
     *
     * @tparam data_t
     * @tparam key_ts
     * @param key
     * @return std::vector<Geometry::Polygon>
     */
  [[nodiscard]] auto get_objectives() const
      -> std::optional<std::map<std::string, std::vector<VIPRA::Geometry::Polygon>>>
  {
    return self().template get_objectives_impl();
  }

  /**
     * @brief Returns polygons as loaded from the input module, std::nullopt if incorrect type / doesn't exist
     *
     * @tparam data_t
     * @tparam key_ts
     * @param key
     * @return std::vector<Geometry::Polygon>
     */
  [[nodiscard]] auto get_areas() const -> std::optional<std::map<std::string, VIPRA::Geometry::Polygon>>
  {
    return self().template get_areas_impl();
  }
};

}  // namespace VIPRA::Modules
