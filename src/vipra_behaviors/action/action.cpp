

#include "badl/actions/action.hpp"

namespace BADL {
void Action::add_call(ActivationCall&& call)
{
  _calls.emplace_back(std::move(call));
}
}  // namespace BADL