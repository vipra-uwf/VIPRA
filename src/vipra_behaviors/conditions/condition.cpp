

#include "badl/cognition/behaviors/conditions/condition.hpp"

namespace BADL {
auto Condition::evaluate(
    BADL::Memory<VIPRA::Sight, VIPRA::Sound> const&        memory,
    BADL::Beliefs<VIPRA::Identity, VIPRA::Position> const& beliefs,
    BADL::Environment<VIPRA::Sound, VIPRA::Sight> const&   environment,
    BADL::time                                             time) -> bool
{
  bool met = _conditions[0](memory, beliefs);

  for ( size_t i = 1; i < _conditions.size(); ++i ) {
    if ( can_short_circuit(met, _steps[i]) ) return met;

    switch ( _steps[i - 1] ) {
      case BoolOp::AND:
        met = met && _conditions[i](memory, beliefs);
        break;
      case BoolOp::OR:
        met = met || _conditions[i](memory, beliefs);
        break;
    }
  }

  return met;
}

void Condition::add_operation(BoolOp oper) { _steps.emplace_back(oper); }
void Condition::add_subcondition(SubCondition&& condition)
{
  _conditions.emplace_back(condition);
}
}  // namespace BADL
