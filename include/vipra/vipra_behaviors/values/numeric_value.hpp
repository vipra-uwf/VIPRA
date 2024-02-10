#pragma once

#include <cmath>
#include <functional>
#include <utility>

#include "vipra/geometry/f3d.hpp"

#include "vipra/vipra_behaviors/definitions/dsl_types.hpp"
#include "vipra/vipra_behaviors/random/random.hpp"

namespace VIPRA::Behaviors {
/**
 * @brief Function for getting the value of a numeric
 * 
 */
using ValueFunc = std::function<float(Behaviors::seed, VIPRA::idx)>;

/**
 * @brief Numeric Values hold runtime float values taken from Behaviors
 * 
 */
class NumericValue {
 public:
  explicit NumericValue(Behaviors::seed seedNum, ValueFunc func) : _seed(seedNum), _val(std::move(func)) {}

  [[nodiscard]] inline auto value(VIPRA::idx pedIdx) const -> float { return _val(_seed, pedIdx); }

 private:
  Behaviors::seed _seed{};
  ValueFunc       _val;

 public:
  NumericValue() = default;
};

/**
 * @brief Holds an exact float value
 * 
 */
struct ExactValue {
  float       value;
  inline auto operator()(Behaviors::seed /*unused*/, VIPRA::idx /*unused*/) const -> float { return value; }
};

/**
 * @brief Holds a random float value for each pedestrian
 * 
 */
struct RandomFloatValue {
  float min{};
  float max{};

  inline auto operator()(Behaviors::seed seed, VIPRA::idx pedIdx) const -> float {
    return Behaviors::DRNG::ped_random_float(seed, pedIdx, Min{min}, Max{max});
  }
};

/**
 * @brief Holds a random whole number value for each pedestrian
 * 
 */
struct RandomNumberValue {
  float min{};
  float max{};

  inline auto operator()(Behaviors::seed seed, VIPRA::idx pedIdx) const -> float {
    return std::round(Behaviors::DRNG::ped_random_float(seed, pedIdx, Min{min}, Max{max}));
  }
};

}  // namespace VIPRA::Behaviors
