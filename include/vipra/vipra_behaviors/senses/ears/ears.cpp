
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
      bool inRange =
          source.position.distance_to_sqrd(pos) < SOUND_MAX_DIST_SQRD_M;

      bool lineOfSight = (interface.map.ray_hit(pos, source.position) ==
                          std::numeric_limits<VIPRA::f_pnt>::max());

      if ( inRange )
        std::cout << "In Range\n";
      else
        std::cout << "NOT In Range\n";

      if ( lineOfSight )
        std::cout << "Line of Sight\n";
      else
        std::cout << "NOT Line of Sight\n";

      return inRange && lineOfSight;
    }
  }

  return false;
}
}  // namespace VIPRA