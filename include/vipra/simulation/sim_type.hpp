#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>

#include "vipra/concepts/all_concepts.hpp"
#include "vipra/concepts/goals.hpp"
#include "vipra/concepts/model.hpp"
#include "vipra/concepts/obstacle_set.hpp"
#include "vipra/concepts/output.hpp"
#include "vipra/concepts/parameters.hpp"
#include "vipra/concepts/pedset.hpp"

#include "vipra/types/parameter.hpp"
#include "vipra/types/timestep.hpp"

namespace VIPRA {

enum class Mode { SINGLE, SWEEP };

template <Concepts::ParamModule params_t, Concepts::OutputModule output_t, Concepts::ModelModule model_t,
          Concepts::PedsetModule pedset_t, Concepts::GoalsModule goals_t, Concepts::ObstacleModule obstacle_t>
class SimType {
 public:
  constexpr SimType(Mode mode, params_t&& params, output_t&& output, model_t&& model, pedset_t&& pedset,
                    goals_t&& goals, obstacle_t&& obstacles)
      : _mode(mode),
        _params(params),
        _output(output),
        _model(model),
        _pedset(pedset),
        _goals(goals),
        _obstacles(obstacles) {}

  void operator()() { setup_params(); }

 private:
  Mode       _mode{Mode::SINGLE};
  params_t   _params;
  output_t   _output;
  model_t    _model;
  pedset_t   _pedset;
  goals_t    _goals;
  obstacle_t _obstacles;

  VIPRA::timestep _timestep{0};

  void setup_params() {
    _params.set_param(Modules::Type::SIMULATION, "max_timestep", Parameter::Type::REQUIRED);
    _params.set_param(Modules::Type::SIMULATION, "timestep_size", Parameter::Type::REQUIRED);
    _model.set_params(_params);
    _output.set_params(_params);
    _pedset.set_params(_params);
    _goals.set_params(_params);
  }

  void run_sim() {
    VIPRA::timestep maxTimestep =
        _params.template get_param<VIPRA::timestep>(Modules::Type::SIMULATION, "max_timestep");

    while (_timestep < maxTimestep) {
      _model.timestep();
    }
  }
};
}  // namespace VIPRA