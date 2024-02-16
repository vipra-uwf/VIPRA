#include <cstddef>
#include <iostream>
#include <tuple>
#include <type_traits>
#include <utility>

#include "vipra/concepts/all_concepts.hpp"

#include "vipra/concepts/output_coordinator.hpp"
#include "vipra/concepts/parameters.hpp"
#include "vipra/random/random.hpp"
#include "vipra/special_modules/behavior_model.hpp"
#include "vipra/types/parameter.hpp"
#include "vipra/types/time.hpp"
#include "vipra/types/util/result_or_void.hpp"

#include "vipra/util/all_of_type.hpp"
#include "vipra/util/debug_do.hpp"

// TODO(rolland): create type erased wrapper for simulation

// TODO(rolland): go through everything and handle errors more gracefully, currently we just throw

namespace VIPRA {

template <Concepts::ParamModule params_t, Concepts::OutputCoordinator output_t, Concepts::ModelModule model_t,
          Concepts::PedsetModule pedset_t, Concepts::GoalsModule goals_t, Concepts::MapModule map_t>
class SimType {
  using base_output_t = decltype(std::declval<output_t>().write());
  // TODO(rolland): break this out
  using output_data_t =
      std::conditional_t<std::is_same_v<base_output_t, void> || std::is_same_v<base_output_t, VOID> ||
                             Util::all_of_type_v<VOID, base_output_t>,
                         void, decltype(std::declval<output_t>().write())>;

 public:
  constexpr SimType(params_t&& params, output_t&& output, model_t&& model, pedset_t&& pedset, goals_t&& goals,
                    map_t&& obstacles)
      : _params(params), _output(output), _model(model), _pedset(pedset), _goals(goals), _map(obstacles) {}

  static void register_params(Concepts::ParamModule auto& params) {
    params.register_param(Modules::Type::SIMULATION, "main", "max_timestep");
    params.register_param(Modules::Type::SIMULATION, "main", "timestep_size");
    params.register_param(Modules::Type::SIMULATION, "main", "output_frequency");
    params.register_param(Modules::Type::SIMULATION, "main", "random_seed");
    BehaviorModel<pedset_t, map_t, goals_t>::register_params(params);
  }

  auto operator()() -> output_data_t {
    if constexpr (std::is_same_v<output_data_t, void>) {
      run_sim();
    } else {
      return run_sim();
    }
  }

  void reconfig() {
    _output.config(_params, _engine);
    _model.config(_params, _engine);
    _pedset.config(_params, _engine);
    _goals.config(_params, _engine);
    _map.config(_params, _engine);
    _behaviorModel.config(_params, _engine);
  }

  auto run_sim() -> output_data_t {
    _output.new_run(_currSimIdx++);
    auto const [maxTimestep, timestepSize, outputFreq, randomseed] = get_sim_params();
    set_params(outputFreq, randomseed);

    _map.initialize(_pedset);
    _goals.initialize(_pedset, _map);
    _model.initialize(_pedset, _map, _goals, _output);

    _behaviorModel.initialize(_pedset, _map, _goals, randomseed);

    VIPRA::State state;
    state.initialize(_pedset);
    while (_timestep < maxTimestep) {
      _model.timestep(_pedset, _map, _goals, _output, state, timestepSize, _timestep);
      _behaviorModel.timestep(_pedset, _map, _goals, state, timestepSize);
      _pedset.update(state);
      _goals.update(_pedset, _map, timestepSize);
      output_positions();
      ++_timestep;
    }

    if constexpr (std::is_same_v<output_data_t, void>) {
      _output.write();
    } else {
      return _output.write();
    }
  }

 private:
  params_t                                _params;
  output_t                                _output;
  model_t                                 _model;
  pedset_t                                _pedset;
  goals_t                                 _goals;
  map_t                                   _map;
  BehaviorModel<pedset_t, map_t, goals_t> _behaviorModel;

  // TODO(rolland): create a sim id and update output to use it

  VIPRA::Random::Engine _engine{};

  VIPRA::idx      _currSimIdx{0};
  VIPRA::timestep _timestep{0};
  VIPRA::timestep _outputFrequency{0};

  // NOLINTNEXTLINE
  void set_params(VIPRA::timestep outputFrequency, VIPRA::size randomSeed) {
    _outputFrequency = outputFrequency;
    _engine.reseed(randomSeed);
  }

  [[nodiscard]] constexpr auto get_sim_params()
      -> std::tuple<VIPRA::f_pnt, VIPRA::f_pnt, VIPRA::timestep, VIPRA::size> {
    VIPRA::timestep maxTimestep = _params.template get_param<VIPRA::timestep>(
        Modules::Type::SIMULATION, "main", "max_timestep", _engine);
    VIPRA::f_pnt timestepSize =
        _params.template get_param<VIPRA::f_pnt>(Modules::Type::SIMULATION, "main", "timestep_size", _engine);
    VIPRA::timestep outputFrequency = _params.template get_param<VIPRA::timestep>(
        Modules::Type::SIMULATION, "main", "output_frequency", _engine);
    VIPRA::size randomSeed =
        _params.template get_param<VIPRA::size>(Modules::Type::SIMULATION, "main", "random_seed", _engine);

    return {maxTimestep, timestepSize, outputFrequency, randomSeed};
  }

  void output_positions() {
    if (_timestep % _outputFrequency != 0) {
      return;
    }

    const VIPRA::size pedCnt = _pedset.num_pedestrians();
    auto const&       coords = _pedset.all_coords();

    for (VIPRA::idx i = 0; i < pedCnt; ++i) {
      _output.ped_timestep_value(i, _timestep / _outputFrequency, "position", coords.at(i));
    }
  }
};
}  // namespace VIPRA