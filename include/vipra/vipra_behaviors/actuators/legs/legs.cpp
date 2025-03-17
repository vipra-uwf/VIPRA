
#include "vipra/vipra_behaviors/actuators/legs.hpp"

#include "badl/agent.hpp"

namespace VIPRA {
void Legs::test(BADL::Agent& agent, BADL::ProgramInterface const&,
                BADL::Environment<VIPRA::Sound, VIPRA::Sight>&,
                BADL::ComponentParams const& params, BADL::time)
{
  std::cout << "TEST: Agent: " << agent.id()
            << " Params: " << params.get<float>(0) << ", " << params.get<int>(1)
            << ", " << params.get<std::string>(2) << '\n';
}

void Legs::stop(BADL::Agent& agent, BADL::ProgramInterface const& interface,
                BADL::Environment<VIPRA::Sound, VIPRA::Sight>&,
                BADL::ComponentParams const&, BADL::time)
{
  interface.state->velocities[agent.id()] = VIPRA::f3d{};
  interface.state->positions[agent.id()] =
      interface.pedset.ped_coords(agent.id());
}
}  // namespace VIPRA
