
#pragma once

#include "badl/components/component.hpp"
#include "badl/components/component_macros.hpp"

#include "vipra/vipra_behaviors/interface/prog_interface.hpp"

namespace VIPRA {
struct Legs : public BADL::Component {
  [[nodiscard]] auto get_activation_grammar() const
      -> std::vector<std::tuple<std::string, std::string_view,
                                BADL::ActivationFunc>> override
  {
    return {
        std::make_tuple("test", "test %flt, %int, %str", &test),
        std::make_tuple("stop", "stop", &stop),
    };
  }

  static void stop(BADL::Agent&, BADL::ProgramInterface const&,
                   BADL::Environment<VIPRA::Sound, VIPRA::Sight>&,
                   BADL::ComponentParams const&, BADL::time);
};
}  // namespace VIPRA