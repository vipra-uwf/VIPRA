#pragma once

#include <vector>

#include "badl/components/behaviors/dsl_behavior.hpp"

namespace BADL {
class Behaviors {
 public:
  [[nodiscard]] auto decide_action(BADL::Agent const&,
                                   BADL::ProgramInterface const&,
                                   BADL::Environment<BADL_STIMULI_TYPES> const&,
                                   BADL::time) -> BADL::Action const&;

  void add_behavior(DSLBehavior const& behavior)
  {
    _behaviors.emplace_back(behavior);
  }

 private:
  std::vector<DSLBehavior> _behaviors;
};
}  // namespace BADL