
#include "vipra/vipra_behaviors/senses/ears.hpp"
#include "badl/agent.hpp"
#include "senses/sound.hpp"

namespace VIPRA {
auto Ears::hears(
    BADL::Agent const& agent, BADL::ProgramInterface const& interface,
    BADL::Environment<VIPRA::Sound, VIPRA::Sight> const& environment,
    BADL::ComponentParams const& params, BADL::time /*unused*/) -> bool
{
  auto const& pos = interface.pedset.ped_coords(agent.id());
  size_t      type = VIPRA::Sound::id(params.get<std::string>(0));

  for ( auto const& source : environment.all_sources<VIPRA::Sound>() ) {
    if ( source.stimulus_id == type ) {
      return (source.position.distance_to_sqrd(pos) < SOUND_MAX_DIST_SQRD_M) &&
             ! (interface.map.ray_hit(pos, source.position) ==
                std::numeric_limits<VIPRA::f_pnt>::max());
    }
  }

  return false;
}
}  // namespace VIPRA