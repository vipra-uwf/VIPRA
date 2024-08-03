#pragma once

#include <stdexcept>

#include "vipra/vipra_behaviors/_grammar/generated/BehaviorParser.h"

#include "vipra/logging/logging.hpp"
#include "vipra/random/random.hpp"

#include "vipra/vipra_behaviors/behavior/exceptions.hpp"
#include "vipra/vipra_behaviors/definitions/dsl_types.hpp"
#include "vipra/vipra_behaviors/time/time.hpp"
#include "vipra/vipra_behaviors/values/numeric_value.hpp"

// TODO(rolland): Check that range values make sense (min < max)

// TODO(rolland): fix parsing of range values (10-15) fails

namespace VIPRA::Behaviors {

using RandomVal = std::pair<VIPRA::f_pnt, VIPRA::f_pnt>;
using RangeVal = std::pair<VIPRA::f_pnt, VIPRA::f_pnt>;

/**
 * @brief Returns a random value in the provided range
 * 
 * @param seed : randomization seed
 * @param min : minimum value
 * @param max : maximum value
 * @return VIPRA::f_pnt 
 */
[[nodiscard]] inline auto collapse_range_value(VIPRA::seed seed, VIPRA::f_pnt min, VIPRA::f_pnt max)
    -> VIPRA::f_pnt
{
  VIPRA::Random::Engine                             eng{seed};
  VIPRA::Random::uniform_distribution<VIPRA::f_pnt> distr{min, max};
  return distr(eng);
}

/**
 * @brief Gets an exact numeric value from a value number context
 * 
 * @param ctx : value context
 * @param seed : randomization seed
 * @return NumericValue 
 */
[[nodiscard]] inline auto get_numeric(BehaviorParser::Value_numberContext* ctx, VIPRA::seed seed)
    -> NumericValue
{
  VIPRA::f_pnt val = std::stof(ctx->NUMBER()->toString());
  return NumericValue(seed, ExactValue{val});
}

/**
 * @brief Gets a random numeric value from a value random context
 * 
 * @param ctx : value context
 * @param seed : randomization seed
 * @return NumericValue 
 */
[[nodiscard]] inline auto get_numeric(BehaviorParser::Value_randomContext* ctx, VIPRA::seed seed)
    -> NumericValue
{
  if ( ctx->random_float() ) {
    auto         numbers = ctx->random_float()->float_range()->FLOAT();
    VIPRA::f_pnt min = std::stof(numbers[0]->toString());
    VIPRA::f_pnt max = std::stof(numbers[1]->toString());
    return NumericValue(seed, RandomFloatValue{min, max});
  }

  auto         numbers = ctx->random_number()->number_range()->NUMBER();
  VIPRA::f_pnt min = std::round(std::stof(numbers[0]->toString()));
  VIPRA::f_pnt max = std::round(std::stof(numbers[1]->toString()));
  return NumericValue(seed, RandomNumberValue{min, max});
}

/**
 * @brief Gets an exact numeric value from a value range context 
 * 
 * @param ctx : value context
 * @param seed : randomization seed
 * @return NumericValue 
 */
[[nodiscard]] inline auto get_numeric(BehaviorParser::Value_rangeContext* ctx, VIPRA::seed seed)
    -> NumericValue
{
  if ( ctx->float_range() ) {
    auto         numbers = ctx->float_range()->FLOAT();
    VIPRA::f_pnt min = std::stof(numbers[0]->toString());
    VIPRA::f_pnt max = std::stof(numbers[1]->toString());
    return NumericValue(seed, ExactValue{collapse_range_value(seed, min, max)});
  }

  auto         numbers = ctx->number_range()->NUMBER();
  VIPRA::f_pnt min = std::round(std::stof(numbers[0]->toString()));
  VIPRA::f_pnt max = std::round(std::stof(numbers[1]->toString()));
  return NumericValue(seed, ExactValue{std::round(collapse_range_value(seed, min, max))});
}

/**
 * @brief Gets an exact numeric value from a value VIPRA::f_pnt context
 * 
 * @param ctx : value context
 * @param seed : not used
 * @return NumericValue 
 */
[[nodiscard]] inline auto get_numeric(BehaviorParser::Value_floatContext* ctx, VIPRA::seed seed)
    -> NumericValue
{
  VIPRA::f_pnt val = std::stof(ctx->FLOAT()->toString());
  return NumericValue(seed, ExactValue{val});
}

/**
 * @brief Gets a numeric value from a numeric value context
 * 
 * @param ctx : value context
 * @param seed : randomization seed
 * @return NumericValue 
 */
[[nodiscard]] inline auto get_numeric(BehaviorParser::Value_numericContext* ctx, VIPRA::seed seed)
    -> NumericValue
{
  if ( ctx->value_float() ) return get_numeric(ctx->value_float(), seed);

  if ( ctx->value_number() ) return get_numeric(ctx->value_number(), seed);

  if ( ctx->value_random() ) return get_numeric(ctx->value_random(), seed);

  if ( ctx->value_range() ) return get_numeric(ctx->value_range(), seed);

  VIPRA::Log::error("Numeric Value Context Missing Children (you should never see this error)");
  std::exit(1);
  return {};
}

/**
 * @brief Creates a f3d from a Value Coord Context
 * 
 * @param ctx : value context
 * @param seed : randomization seed
 * @return VIPRA::f3d 
 */
[[nodiscard]] inline auto get_coord(BehaviorParser::Value_coordContext* ctx, VIPRA::seed seed) -> VIPRA::f3d
{
  auto       values = ctx->value_numeric();
  VIPRA::f3d val;

  for ( VIPRA::idx i = 0; i < values.size(); ++i ) {
    auto num = get_numeric(values[i], seed);
    val[i] = num.value(0);
  }

  return val;
}

}  // namespace VIPRA::Behaviors
