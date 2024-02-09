#ifndef VIPRA_BEHAVIORS_NUMERIC_VALUE
#define VIPRA_BEHAVIORS_NUMERIC_VALUE

#include <cmath>
#include <functional>
#include <utility>

#include "definitions/dsl_types.hpp"
#include "random/random.hpp"
#include "vipra/geometry/f3d.hpp"

namespace BHVR {
/**
 * @brief Function for getting the value of a numeric
 * 
 */
using ValueFunc = std::function<float(BHVR::seed, VIPRA::idx)>;

/**
 * @brief Numeric Values hold runtime float values taken from Behaviors
 * 
 */
class NumericValue {
 public:
  explicit NumericValue(BHVR::seed seedNum, ValueFunc func) : _seed(seedNum), _val(std::move(func)) {}

  [[nodiscard]] inline auto value(VIPRA::idx pedIdx) const -> float { return _val(_seed, pedIdx); }

 private:
  BHVR::seed _seed{};
  ValueFunc  _val;

 public:
  NumericValue() = default;
};

/**
 * @brief Holds an exact float value
 * 
 */
struct ExactValue {
  float       value;
  inline auto operator()(BHVR::seed /*unused*/, VIPRA::idx /*unused*/) const -> float { return value; }
};

/**
 * @brief Holds a random float value for each pedestrian
 * 
 */
struct RandomFloatValue {
  float min{};
  float max{};

  inline auto operator()(BHVR::seed seed, VIPRA::idx pedIdx) const -> float {
    return BHVR::DRNG::ped_random_float(seed, pedIdx, Min{min}, Max{max});
  }
};

/**
 * @brief Holds a random whole number value for each pedestrian
 * 
 */
struct RandomNumberValue {
  float min{};
  float max{};

  inline auto operator()(BHVR::seed seed, VIPRA::idx pedIdx) const -> float {
    return std::round(BHVR::DRNG::ped_random_float(seed, pedIdx, Min{min}, Max{max}));
  }
};

}  // namespace BHVR

#endif