#pragma once

#include "badl/components/component.hpp"
#include "vipra/vipra_behaviors/interface/prog_interface.hpp"

namespace VIPRA {

class Mouth : public BADL::Component {
 public:
  auto get_actuator_grammar()
      -> std::vector<std::pair<std::string_view, BADL::ActuatorFunc>> override
  {
    return {
        std::make_pair("say %str", &say),
    };
  }

  static void say(BADL::ProgramInterface const&,
                  BADL::Environment<VIPRA::Sound, VIPRA::Sight>&,
                  BADL::ComponentParams const&)
  {
  }
};

}  // namespace VIPRA