#include <vipra.hpp>

#include "../modules/model/calm_model/calm_model.hpp"

auto main() -> int {
  auto sim = VIPRA::simulation(
      VIPRA::Module::Output{
        VIPRA::Output::Trajectories::JSON{}
      },
      CALM::Model{},
      VIPRA::Goals::AStar{},
      VIPRA::Pedestrians::Grid{
          VIPRA::Input::JSON{"../maps/pedestrians/a320/a320_144_pedestrians.json"},
      },
      VIPRA::Module::Map{
        VIPRA::Input::JSON{"../maps/obstacles/a320/a320_polygons.json"},
        VIPRA::Obstacles::QuadTree{}
      }
  );
  
  sim(VIPRA::Parameters{VIPRA::Input::JSON{"./module_params.json"}});
}