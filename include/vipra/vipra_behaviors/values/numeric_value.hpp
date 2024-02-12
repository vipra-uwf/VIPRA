#pragma once

#include <cmath>
#include <functional>
#include <utility>

#include "vipra/geometry/f3d.hpp"
#include "vipra/random/random.hpp"
#include "vipra/types/idx.hpp"

#include "vipra/vipra_behaviors/definitions/dsl_types.hpp"
#include "vipra/vipra_behaviors/random/random.hpp"

namespace VIPRA::Behaviors {
/**
 * @brief Function for getting the value of a numeric
 * 
 */
using ValueFunc = std::function<VIPRA::f_pnt(VIPRA::seed, VIPRA::idx)>;

/**
 * @brief Numeric Values hold runtime VIPRA::f_pnt values taken from Behaviors
 * 
 */
class NumericValue {
 public:
  explicit NumericValue(VIPRA::seed seedNum, ValueFunc func) : _seed(seedNum), _val(std::move(func)) {}

  [[nodiscard]] inline auto value(VIPRA::idx pedIdx) const -> VIPRA::f_pnt { return _val(_seed, pedIdx); }

 private:
  VIPRA::seed _seed{};
  ValueFunc   _val;

 public:
  NumericValue() = default;
};

/**
 * @brief Holds an exact VIPRA::f_pnt value
 * 
 */
struct ExactValue {
  VIPRA::f_pnt value;
  inline auto  operator()(VIPRA::seed /*unused*/, VIPRA::idx /*unused*/) const -> VIPRA::f_pnt {
    return value;
  }
};

/**
 * @brief Holds a random VIPRA::f_pnt value for each pedestrian
 * 
 */
struct RandomFloatValue {
  VIPRA::f_pnt min{};
  VIPRA::f_pnt max{};

  inline auto operator()(VIPRA::seed seed, VIPRA::idx pedIdx) const -> VIPRA::f_pnt {
    return Behaviors::DRNG::ped_random_float(seed, pedIdx, Min{min}, Max{max});
  }
};

/**
 * @brief Holds a random whole number value for each pedestrian
 * 
 */
struct RandomNumberValue {
  VIPRA::f_pnt min{};
  VIPRA::f_pnt max{};

  inline auto operator()(VIPRA::seed seed, VIPRA::idx pedIdx) const -> VIPRA::f_pnt {
    return std::round(Behaviors::DRNG::ped_random_float(seed, pedIdx, Min{min}, Max{max}));
  }
};

}  // namespace VIPRA::Behaviors
