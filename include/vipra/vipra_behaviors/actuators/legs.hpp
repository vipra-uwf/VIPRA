
#pragma once

#include "badl/components/component.hpp"
#include "vipra/vipra_behaviors/interface/prog_interface.hpp"

namespace VIPRA {
struct Legs : public BADL::Component {
  auto get_actuator_grammar()
      -> std::vector<std::pair<std::string_view, BADL::ActivationFunc>> override
  {
    return {
        std::make_pair("move %flt, %flt, %flt", &move),
        std::make_pair("stop", &stop),
    };
  }

  static void move(BADL::Agent&, BADL::ProgramInterface const&,
                   BADL::Environment<VIPRA::Sound, VIPRA::Sight>&,
                   BADL::ComponentParams const&, BADL::time)
  {
  }

  static void stop(BADL::Agent&, BADL::ProgramInterface const&,
                   BADL::Environment<VIPRA::Sound, VIPRA::Sight>&,
                   BADL::ComponentParams const&, BADL::time)
  {
  }
};
}  // namespace VIPRA