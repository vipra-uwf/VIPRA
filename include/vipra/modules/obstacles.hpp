#pragma once

#include <concepts>
#include <map>
#include <utility>
#include <vector>

#include "vipra/modules/module.hpp"

#include "vipra/geometry/f3d.hpp"
#include "vipra/geometry/polygon.hpp"

#include "vipra/types/float.hpp"
#include "vipra/util/crtp.hpp"

namespace VIPRA::Modules {
/**
 * @brief Dummy obstacle set for use in other concepts
 * 
 */
template <typename module_t>
class Obstacles : public Util::CRTP<Obstacles<module_t>> {
  using Util::CRTP<Obstacles<module_t>>::self;

  // NOLINTBEGIN
 public:
  void initialize(auto&& input)
  {
    // load object types from input
    auto const objTypes = input.template get<std::vector<std::string>>("obj_types");
    if ( ! objTypes ) throw std::runtime_error("Could not find object types in input file");

    // load object locations for each type
    auto objMap = std::map<std::string, std::vector<VIPRA::f3d>>{};
    std::for_each(objTypes->begin(), objTypes->end(), [&](auto const& objType) {
      const auto positions = input.template get<std::vector<VIPRA::f3d>>(objType);
      if ( ! positions ) {
        throw std::runtime_error("Could not get object positions from input");
      }

      std::for_each(positions->begin(), positions->end(),
                    [&](auto const& objPos) { objMap[objType].push_back(objPos); });
    });

    // load obstacle polygons
    auto const obsCoords = input.template load_polygons("obstacles");
    if ( ! obsCoords ) throw std::runtime_error("Could not get obstacle polygons from input");

    // initialize derived obstacles module
    return self().init_step(obsCoords.value(), objTypes.value(), objMap);
  }

  [[nodiscard]] auto get_dimensions() const -> VIPRA::f3d { return self().get_dimensions(); }

  [[nodiscard]] auto collision(VIPRA::f3d point) const -> bool { return self().collision(point); }

  [[nodiscard]] auto collision(VIPRA::Geometry::Circle radius) const -> bool
  {
    return self().collision(radius);
  }

  [[nodiscard]] auto ray_hit(VIPRA::f3d start, VIPRA::f3d end) const -> VIPRA::f_pnt
  {
    return self().ray_hit(start, end);
  }

  [[nodiscard]] auto get_object_types() const -> std::vector<std::string> const&
  {
    return self().get_object_types();
  }

  [[nodiscard]] auto get_objects(std::string const& type) const -> const VIPRA::f3dVec&
  {
    return self().get_objects(type);
  }
};

}  // namespace VIPRA::Modules