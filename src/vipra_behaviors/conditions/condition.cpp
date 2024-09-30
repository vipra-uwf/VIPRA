#pragma once

#include "vipra/vipra_behaviors/conditions/condition.hpp"

namespace VIPRA::Behaviors {

/**
   * @brief Evaluates whether a condition is met for a group of pedestrians, writes to 'met' vector
   * 
   * @tparam Simpack 
   * @param pack 
   * @param peds 
   * @param met - output vector
   * @param targets
   * @param latches 
   */
void Condition::evaluate(Simpack pack, VIPRA::idxVec const& peds,
                         std::vector<bool>&                   met,
                         std::vector<Target> const&           targets,
                         std::optional<TimedLatchCollection>& latches)
{
  std::fill(_temp.begin(), _temp.end(), false);
  _conditions[0](pack, peds, targets, met, _temp, BoolOp::OR);

  for ( VIPRA::idx i = 1; i < _conditions.size(); ++i ) {
    _conditions[i](pack, peds, targets, _temp, met, _steps[i - 1]);
    switch ( _steps[i - 1] ) {
      case BoolOp::AND:
        std::transform(met.begin(), met.end(), _temp.begin(), met.begin(),
                       std::logical_and<>());
        break;
      case BoolOp::OR:
        std::transform(met.begin(), met.end(), _temp.begin(), met.begin(),
                       std::logical_or<>());
        break;
    }
  }

  handle_latches(pack, latches, met);
}

/**
   * @brief Updates action duration latches
   * 
   * @tparam Simpack 
   * @param pack 
   * @param latches 
   * @param met 
   */
void Condition::handle_latches(Simpack                              pack,
                               std::optional<TimedLatchCollection>& latches,
                               std::vector<bool>&                   met)
{
  if ( latches.has_value() ) {
    for ( VIPRA::idx i = 0; i < met.size(); ++i ) {
      if ( met[i] ) {
        latches->latch(pack.dT, i);
      }

      met[i] = latches->check(pack.dT, i);
    }
  }
}

}  // namespace VIPRA::Behaviors
