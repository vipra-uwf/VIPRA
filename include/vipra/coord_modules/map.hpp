#pragma once

#include "vipra/concepts/input.hpp"
#include "vipra/concepts/map.hpp"
#include "vipra/concepts/obstacle_set.hpp"
#include "vipra/concepts/polygon_input.hpp"

#include "vipra/geometry/circle.hpp"
#include "vipra/modules.hpp"
#include "vipra/random/random.hpp"

namespace VIPRA::Module {
template <Concepts::PolygonInput input_t, Concepts::ObstacleModule obstacles_t>
class Map {
 public:
  // NOLINTNEXTLINE
  constexpr static VIPRA::Modules::Type _VIPRA_MODULE_TYPE_ = VIPRA::Modules::Type::MAP;
  ;

  /**
   * @brief Construct a new Map object, loads obstacles from input file
   * 
   * @param params 
   */
  explicit Map(input_t&& input, obstacles_t&& obstacles) : _input(input), _obstacles{std::move(obstacles)} {}

  /**
   * @brief Initializes all fields with the pedestrians and obstacles
   * 
   * @tparam pedestrians_t 
   * @param pedestrians 
   */
  void initialize() {
    _input.load();
    initialize_obstacles();
  }

  /**
   * @brief Calls setup on all obstacles modules
   * 
   * @param params 
   */
  void config(auto const& params, VIPRA::Random::Engine& engine) { _obstacles.config(params, engine); }

  /**
   * @brief Calls register_params on all obstacles modules
   * 
   * @tparam params_t
   */
  template <Concepts::ParamModule params_t>
  void register_params(params_t& params) {
    _obstacles.register_params(params);
  }

  [[nodiscard]] inline auto get_object_types() const -> std::vector<std::string> {
    return _obstacles.get_object_types();
  }

  [[nodiscard]] constexpr inline auto get_objects(std::string const& type) const -> const VIPRA::f3dVec& {
    return _obstacles.get_objects(type);
  }

  [[nodiscard]] constexpr inline auto get_dimensions() const -> VIPRA::f3d {
    return _obstacles.get_dimensions();
  }

  [[nodiscard]] constexpr inline auto collision(VIPRA::f3d point) const -> bool {
    return _obstacles.collision(point);
  }

  [[nodiscard]] constexpr inline auto collision(VIPRA::Geometry::Circle point) const -> bool {
    return _obstacles.collision(point);
  }

  [[nodiscard]] constexpr inline auto ray_hit(VIPRA::f3d start, VIPRA::f3d end) const -> VIPRA::f_pnt {
    return _obstacles.ray_hit(start, end);
  }

  [[nodiscard]] inline auto obstacle_set() const -> obstacles_t const& { return _obstacles; }

 private:
  obstacles_t _obstacles{};
  input_t     _input;

  void initialize_obstacles() {
    auto const objTypes = _input.template get<std::vector<std::string>>("obj_types");
    if (!objTypes) throw std::runtime_error("Could not find object types in input file");

    auto objMap = std::map<std::string, std::vector<VIPRA::f3d>>{};

    std::for_each(objTypes->begin(), objTypes->end(), [&](auto const& objType) {
      const auto positions = _input.template get<std::vector<VIPRA::f3d>>(objType);
      if (!positions) {
        throw std::runtime_error("Could not get object positions from input");
      }

      std::for_each(positions->begin(), positions->end(),
                    [&](const auto& objPos) { objMap[objType].push_back(objPos); });
    });

    auto const obsCoords = _input.template load_polygons("obstacles");
    if (!obsCoords) throw std::runtime_error("Could not get obstacle polygons from input");

    _obstacles.initialize(obsCoords.value(), objTypes.value(), objMap);
  }
};

CHECK_MODULE(MapModule, Map<Concepts::DummyPolygonInput, Concepts::DummyObsSet>)
}  // namespace VIPRA::Module