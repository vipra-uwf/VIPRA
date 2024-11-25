#pragma once

#include <optional>
#include <type_traits>
#include <vector>

#include "vipra/geometry/polygon.hpp"
#include "vipra/modules/module.hpp"
#include "vipra/util/crtp.hpp"

namespace VIPRA::Modules {

/**
 * @brief Dummy polygon input for use in other concepts
 * 
 */
template <typename module_t>
class PolygonInput : public Util::CRTP<PolygonInput<module_t>> {
  using Util::CRTP<PolygonInput<module_t>>::self;

 public:
  /**
     * @brief Returns polygons as loaded from the input module, std::nullopt if incorrect type / doesn't exist
     *
     * @tparam data_t
     * @tparam key_ts
     * @param key
     * @return std::vector<Geometry::Polygon>
     */
  template <typename... keys_t>
  [[nodiscard]] auto load_polygons(keys_t&&... keys) const
      -> std::optional<std::vector<VIPRA::Geometry::Polygon>>
  {
    return self().template get<std::vector<VIPRA::Geometry::Polygon>>(
        std::forward<keys_t>(keys)...);
  }
};

}  // namespace VIPRA::Modules
