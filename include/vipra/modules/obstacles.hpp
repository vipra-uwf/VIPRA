#pragma once

#include <concepts>
#include <map>
#include <utility>
#include <vector>

#include "vipra/modules/module.hpp"

#include "vipra/geometry/f3d.hpp"
#include "vipra/geometry/polygon.hpp"

#include "vipra/types/float.hpp"

namespace VIPRA::Modules {
/**
 * @brief Dummy obstacle set for use in other concepts
 * 
 */
template <typename module_t>
class Obstacles : public Module<Obstacles<module_t>> {
  // NOLINTBEGIN
 public:
  void initialize(std::vector<VIPRA::Geometry::Polygon> const&          obstacles,
                  std::vector<std::string> const&                       types,
                  std::map<std::string, std::vector<VIPRA::f3d>> const& objects) {
    return this->self().init_step(obstacles, types, objects);
  }

  [[nodiscard]] auto get_dimensions() const -> VIPRA::f3d { return this->self().get_dimensions(); }

  [[nodiscard]] auto collision(VIPRA::f3d point) const -> bool { return this->self().collision(point); }

  [[nodiscard]] auto collision(VIPRA::Geometry::Circle radius) const -> bool {
    return this->self().collision(radius);
  }

  [[nodiscard]] auto ray_hit(VIPRA::f3d start, VIPRA::f3d end) const -> VIPRA::f_pnt {
    return this->self().ray_hit(start, end);
  }

  [[nodiscard]] auto get_object_types() const -> std::vector<std::string> const& {
    return this->self().get_object_types();
  }

  [[nodiscard]] auto get_objects(std::string const& type) const -> const VIPRA::f3dVec& {
    return this->self().get_objects(type);
  }
};

}  // namespace VIPRA::Modules