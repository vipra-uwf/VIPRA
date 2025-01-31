

#include "vipra/vipra_behaviors/actuators/mouth.hpp"
#include "badl/agent.hpp"

namespace VIPRA {
void Mouth::say(BADL::Agent& agent, BADL::ProgramInterface const& interface,
                BADL::Environment<VIPRA::Sound, VIPRA::Sight>& environment,
                BADL::ComponentParams const& params, BADL::time /*unused*/)
{
  auto sound = VIPRA::Sound(VIPRA::Sound::id(params.get<std::string>(0)),
                            interface.pedset.ped_coords(agent.id()));
  environment.add_source<VIPRA::Sound>(sound);
}
}  // namespace VIPRA