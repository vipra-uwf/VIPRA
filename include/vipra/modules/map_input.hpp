#pragma once

#include <optional>
#include <type_traits>
#include <vector>

#include "vipra/geometry/polygon.hpp"
#include "vipra/modules/module.hpp"
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
  /**
     * @brief Returns polygons as loaded from the input module, std::nullopt if incorrect type / doesn't exist
     *
     * @tparam data_t
     * @tparam key_ts
     * @param key
     * @return std::vector<Geometry::Polygon>
     */
  [[nodiscard]] auto load_polygons(std::vector<std::string> const& keys) const
      -> std::optional<std::vector<VIPRA::Geometry::Polygon>>
  {
    return self().template get<std::vector<VIPRA::Geometry::Polygon>>(keys);
  }
};

}  // namespace VIPRA::Modules
