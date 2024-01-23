#pragma once

#include "vipra/concepts/field.hpp"
#include "vipra/concepts/input.hpp"
#include "vipra/concepts/map.hpp"
#include "vipra/concepts/obstacle_set.hpp"

#include "vipra/modules.hpp"

namespace VIPRA {
template <Concepts::ObstacleModule obstacles_t, Concepts::FieldModule... field_ts>
class Map {
  VIPRA_MODULE_TYPE(MAP);

 public:
  /**
   * @brief Construct a new Map object
   * 
   * @param params 
   */
  template <Concepts::InputModule input_t>
  explicit constexpr Map(const input_t& input) {
    // TODO: load map from input
  }

  /**
   * @brief Calls setup on all obstacles modules
   * 
   * @param params 
   */
  void setup(const auto& params) { _obstacles.setup(params); }

  /**
   * @brief Calls register_params on all obstacles modules
   * 
   * @tparam params_t
   */
  template <Concepts::ParamModule params_t>
  static void register_params() {
    obstacles_t::template register_params<params_t>();
  }

 private:
  obstacles_t             _obstacles{};
  std::tuple<field_ts...> _fields;
};

// CHECK_MODULE(MapModule, Map<Concepts::DummyObsSet, Concepts::DummyField>)
}  // namespace VIPRA