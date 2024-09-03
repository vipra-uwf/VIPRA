#pragma once

#include "vipra/modules/map.hpp"

namespace VIPRA::Views {
class Map : public Modules::Map<Map> {
  using get_dimensions_impl_func_t = VIPRA::f3d(void const*);
  using f3d_collision_impl_func_t = bool(void const*, VIPRA::f3d);
  using circle_collision_impl_func_t = bool(void const*, VIPRA::Geometry::Circle);
  using ray_hit_impl_func_t = VIPRA::f_pnt(void const*, VIPRA::f3d, VIPRA::f3d);
  using get_object_types_func_t = std::vector<std::string> const&(void const*);
  using get_objects_func_t = const VIPRA::f3dVec&(void const*, std::string const&);

 private:
  void const*                   _module;
  get_dimensions_impl_func_t*   _get_dimensions_impl;
  f3d_collision_impl_func_t*    _f3d_collision_impl;
  circle_collision_impl_func_t* _circle_collision_impl;
  ray_hit_impl_func_t*          _ray_hit_impl;
  get_object_types_func_t*      _get_object_types;
  get_objects_func_t*           _get_objects;

 public:
  template <typename module_t>
  // NOLINTNEXTLINE(google-explicit-constructor)
  Map(module_t const& module) : _module(&module)
  {
    // NOLINTBEGIN(cppcoreguidelines-prefer-member-initializer)

    _get_dimensions_impl = +[](void const* ptr) -> VIPRA::f3d {
      return static_cast<module_t const*>(ptr)->get_dimensions_impl();
    };
    _f3d_collision_impl = +[](void const* ptr, VIPRA::f3d point) -> bool {
      return static_cast<module_t const*>(ptr)->collision_impl(point);
    };
    _circle_collision_impl = +[](void const* ptr, VIPRA::Geometry::Circle circle) -> bool {
      return static_cast<module_t const*>(ptr)->collision_impl(circle);
    };
    _ray_hit_impl = +[](void const* ptr, VIPRA::f3d start, VIPRA::f3d end) -> VIPRA::f_pnt {
      return static_cast<module_t const*>(ptr)->ray_hit_impl(start, end);
    };
    _get_object_types = +[](void const* ptr) -> std::vector<std::string> const& {
      return static_cast<module_t const*>(ptr)->get_object_types();
    };
    _get_objects = +[](void const* ptr, std::string const& type) -> const VIPRA::f3dVec& {
      return static_cast<module_t const*>(ptr)->get_objects(type);
    };

    // NOLINTEND(cppcoreguidelines-prefer-member-initializer)
  }

  [[nodiscard]] auto get_dimensions_impl() const -> VIPRA::f3d { return _get_dimensions_impl(_module); }
  [[nodiscard]] auto collision_impl(VIPRA::f3d point) const -> bool
  {
    return _f3d_collision_impl(_module, point);
  }
  [[nodiscard]] auto collision_impl(VIPRA::Geometry::Circle radius) const -> bool
  {
    return _circle_collision_impl(_module, radius);
  }
  [[nodiscard]] auto ray_hit_impl(VIPRA::f3d start, VIPRA::f3d end) const -> VIPRA::f_pnt
  {
    return _ray_hit_impl(_module, start, end);
  }
  [[nodiscard]] auto get_object_types() const -> std::vector<std::string> const&
  {
    return _get_object_types(_module);
  }
  [[nodiscard]] auto get_objects(std::string const& type) const -> const VIPRA::f3dVec&
  {
    return _get_objects(_module, type);
  }
};
}  // namespace VIPRA::Views