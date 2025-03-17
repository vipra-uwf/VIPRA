#pragma once

#include "badl/components/component.hpp"

namespace VIPRA {
class Eyes : public BADL::Component {
 public:
  [[nodiscard]] auto get_query_grammar() const
      -> std::vector<
          std::tuple<std::string, std::string_view, BADL::QueryFunc>> override
  {
    return {
        std::make_tuple("sees", "sees %str", &sees),
    };
  }

  static auto sees(
      BADL::Agent const& agent, BADL::ProgramInterface const& interface,
      BADL::Environment<VIPRA::Sound, VIPRA::Sight> const& environment,
      BADL::ComponentParams const& params, BADL::time) -> bool;
};
}  // namespace VIPRA