
#pragma once

#include "badl/components/component.hpp"

#include "vipra/vipra_behaviors/interface/prog_interface.hpp"

namespace VIPRA {
struct Brain : public BADL::Component {
  [[nodiscard]] auto get_activation_grammar() const
      -> std::vector<std::tuple<std::string, std::string_view,
                                BADL::ActivationFunc>> override
  {
    return {
        std::make_tuple("change_goal", "change goal to %flt, %flt, %flt",
                        &change_goal_f3d),
        std::make_tuple("change_goal", "change goal to %str", &change_goal_pos),
    };
  }

  static void change_goal_f3d(BADL::Agent&, BADL::ProgramInterface const&,
                              BADL::Environment<VIPRA::Sound, VIPRA::Sight>&,
                              BADL::ComponentParams const&, BADL::time);
  static void change_goal_pos(BADL::Agent&, BADL::ProgramInterface const&,
                              BADL::Environment<VIPRA::Sound, VIPRA::Sight>&,
                              BADL::ComponentParams const&, BADL::time);
};
}  // namespace VIPRA