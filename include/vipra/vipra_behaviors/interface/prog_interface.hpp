#pragma once

#include "vipra/modules/goals.hpp"
#include "vipra/modules/map.hpp"
#include "vipra/modules/pedestrians.hpp"

namespace BADL {
struct ProgramInterface {
  // NOLINTBEGIN(cppcoreguidelines-avoid-const-or-ref-data-members)
  VIPRA::Modules::Pedestrians const& pedset;
  VIPRA::Modules::Goals const&       goals;
  VIPRA::Modules::Map const&         map;
  VIPRA::State*                      state{nullptr};
  // NOLINTEND(cppcoreguidelines-avoid-const-or-ref-data-members)
};
}  // namespace BADL