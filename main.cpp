
#include <vipra.hpp>

#include "modules/model/calm_model.hpp"
#include "vipra/concepts/model.hpp"

auto main() -> int {
  VIPRA::simulation(
    VIPRA::Mode::SWEEP,
    VIPRA::Parameters {
      VIPRA::Input::JSON{"module_params.json"}
    },
    VIPRA::Output::JSON{"output.json"},
    VIPRA::Goals::AStar{},
    CALM::Model{},
    VIPRA::Obstacles{
      VIPRA::Input::JSON{"obstacles.json"}
    },
    VIPRA::Pedestrians{
      VIPRA::Input::JSON{"pedestrians.json"}
    }
  )();
}