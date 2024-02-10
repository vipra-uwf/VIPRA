#pragma once

#include <stdexcept>

#include <generated/BehaviorParser.h>

#include "behavior/exceptions.hpp"
#include "definitions/dsl_types.hpp"
#include "randomization/random.hpp"
#include "time/time.hpp"
#include "values/numeric_value.hpp"

// TODO: Check that range values make sense (min < max)

namespace VIPRA::Behaviors {

using RandomVal = std::pair<float, float>;
using RangeVal = std::pair<float, float>;

/**
 * @brief Returns a random value in the provided range
 * 
 * @param seed : randomization seed
 * @param min : minimum value
 * @param max : maximum value
 * @return float 
 */
[[nodiscard]] inline auto collapse_range_value(Behaviors::seed seed, float min, float max) -> float {
  VIPRA::pRNG_Engine                 eng{seed};
  VIPRA::uniform_distribution<float> distr{min, max};
  return distr(eng);
}

/**
 * @brief Gets an exact numeric value from a value number context
 * 
 * @param ctx : value context
 * @param seed : randomization seed
 * @return NumericValue 
 */
[[nodiscard]] inline auto get_numeric(BehaviorParser::Value_numberContext* ctx, Behaviors::seed seed)
    -> NumericValue {
  float val = std::stof(ctx->NUMBER()->toString());
  return NumericValue(seed, ExactValue{val});
}

/**
 * @brief Gets a random numeric value from a value random context
 * 
 * @param ctx : value context
 * @param seed : randomization seed
 * @return NumericValue 
 */
[[nodiscard]] inline auto get_numeric(BehaviorParser::Value_randomContext* ctx, Behaviors::seed seed)
    -> NumericValue {
  if (ctx->random_float()) {
    auto  numbers = ctx->random_float()->float_range()->FLOAT();
    float min = std::stof(numbers[0]->toString());
    float max = std::stof(numbers[1]->toString());
    return NumericValue(seed, RandomFloatValue{min, max});
  }

  auto  numbers = ctx->random_number()->number_range()->NUMBER();
  float min = std::round(std::stof(numbers[0]->toString()));
  float max = std::round(std::stof(numbers[1]->toString()));
  return NumericValue(seed, RandomNumberValue{min, max});
}

/**
 * @brief Gets an exact numeric value from a value range context 
 * 
 * @param ctx : value context
 * @param seed : randomization seed
 * @return NumericValue 
 */
[[nodiscard]] inline auto get_numeric(BehaviorParser::Value_rangeContext* ctx, Behaviors::seed seed)
    -> NumericValue {
  if (ctx->float_range()) {
    auto  numbers = ctx->float_range()->FLOAT();
    float min = std::stof(numbers[0]->toString());
    float max = std::stof(numbers[1]->toString());
    return NumericValue(seed, ExactValue{collapse_range_value(seed, min, max)});
  }

  auto  numbers = ctx->number_range()->NUMBER();
  float min = std::round(std::stof(numbers[0]->toString()));
  float max = std::round(std::stof(numbers[1]->toString()));
  return NumericValue(seed, ExactValue{std::round(collapse_range_value(seed, min, max))});
}

/**
 * @brief Gets an exact numeric value from a value float context
 * 
 * @param ctx : value context
 * @param seed : not used
 * @return NumericValue 
 */
[[nodiscard]] inline auto get_numeric(BehaviorParser::Value_floatContext* ctx, Behaviors::seed seed)
    -> NumericValue {
  float val = std::stof(ctx->FLOAT()->toString());
  return NumericValue(seed, ExactValue{val});
}

/**
 * @brief Gets a numeric value from a numeric value context
 * 
 * @param ctx : value context
 * @param seed : randomization seed
 * @return NumericValue 
 */
[[nodiscard]] inline auto get_numeric(BehaviorParser::Value_numericContext* ctx, Behaviors::seed seed)
    -> NumericValue {
  if (ctx->value_float()) return get_numeric(ctx->value_float(), seed);

  if (ctx->value_number()) return get_numeric(ctx->value_number(), seed);

  if (ctx->value_random()) return get_numeric(ctx->value_random(), seed);

  if (ctx->value_range()) return get_numeric(ctx->value_range(), seed);

  // spdlog::error("Numeric Value Context Missing Children (you should never see this error)");
  throw std::runtime_error("");
  return {};
}

/**
 * @brief Creates a f3d from a Value Coord Context
 * 
 * @param ctx : value context
 * @param seed : randomization seed
 * @return VIPRA::f3d 
 */
[[nodiscard]] inline auto get_coord(BehaviorParser::Value_coordContext* ctx, Behaviors::seed seed)
    -> VIPRA::f3d {
  auto       values = ctx->value_numeric();
  VIPRA::f3d val;

  for (VIPRA::idx i = 0; i < values.size(); ++i) {
    auto num = get_numeric(values[i], seed);
    val[i] = num.value(0);
  }

  return val;
}

}  // namespace VIPRA::Behaviors
