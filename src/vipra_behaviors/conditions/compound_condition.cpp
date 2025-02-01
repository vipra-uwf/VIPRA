

#include "badl/components/behaviors/conditions/compound_condition.hpp"

namespace BADL {
auto CompoundCondition::evaluate(
    BADL::Agent const& agent, BADL::ProgramInterface const& interface,
    BADL::Environment<VIPRA::Sound, VIPRA::Sight> const& environment,
    BADL::time                                           time) const -> bool
{
  bool met = _conditions[0](agent, interface, environment, time);

  for ( size_t i = 1; i < _conditions.size(); ++i ) {
    if ( can_short_circuit(met, _steps[i]) ) return met;

    switch ( _steps[i - 1] ) {
      case BoolOp::AND:
        met = met && _conditions[i](agent, interface, environment, time);
        break;
      case BoolOp::OR:
        met = met || _conditions[i](agent, interface, environment, time);
        break;
    }
  }

  return met;
}

void CompoundCondition::add_query(QueryCall const& call)
{
  _conditions.emplace_back(call);
}

}  // namespace BADL
