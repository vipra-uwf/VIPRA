#pragma once

#include "badl/components/component.hpp"
#include "badl/components/component_methods.hpp"
#include "beliefs/position.hpp"

namespace VIPRA {
class Ears : public BADL::Component {
 public:
  [[nodiscard]] auto get_query_grammar() const
      -> std::vector<
          std::tuple<std::string, std::string_view, BADL::QueryFunc>> override
  {
    return {
        std::make_tuple("hear", "hears %str", &hears),
    };
  }

  static auto hears(BADL::Agent const&            agent,
                    BADL::ProgramInterface const& interface,
                    BADL::Environment<VIPRA::Sound, VIPRA::Sight> const&,
                    BADL::ComponentParams const&, BADL::time) -> bool;

 private:
  static constexpr double SOUND_MAX_DIST_SQRD_M = 25.0;
};
}  // namespace VIPRA