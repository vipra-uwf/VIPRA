
#include "badl/agent.hpp"

#include "vipra/vipra_behaviors/senses/eyes.hpp"

namespace VIPRA {
auto Eyes::sees(
    BADL::Agent const& agent, BADL::ProgramInterface const& interface,
    BADL::Environment<VIPRA::Sound, VIPRA::Sight> const& environment,
    BADL::ComponentParams const& params, BADL::time /*unused*/) -> bool
{
  auto const& pos = interface.pedset.ped_coords(agent.id());
  size_t      type = VIPRA::Sight::id(params.get<std::string>(0));

  for ( auto const& source : environment.all_sources<VIPRA::Sight>() ) {
    if ( type == source.stimulus_id )
      return ! (interface.map.ray_hit(pos, source.position) ==
                std::numeric_limits<VIPRA::f_pnt>::max());
  }

  return false;
}
}  // namespace VIPRA