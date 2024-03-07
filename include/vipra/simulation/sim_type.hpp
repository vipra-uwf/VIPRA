#include <cstddef>
#include <iostream>
#include <tuple>
#include <type_traits>
#include <utility>

#include "vipra/concepts/all_concepts.hpp"

#include "vipra/concepts/output_coordinator.hpp"
#include "vipra/concepts/parameters.hpp"
#include "vipra/parameter_sweep/parameter_sweep.hpp"
#include "vipra/random/random.hpp"
#include "vipra/special_modules/behavior_model.hpp"
#include "vipra/types/parameter.hpp"
#include "vipra/types/time.hpp"

#include "vipra/types/util/result_or_void.hpp"
#include "vipra/types/util/sim_output.hpp"

#include "vipra/util/all_of_type.hpp"
#include "vipra/util/debug_do.hpp"

// TODO(rolland): create type erased wrapper for simulation

// TODO(rolland): go through everything and handle errors more gracefully, currently we just throw

namespace VIPRA {

template <Concepts::OutputCoordinator output_t, Concepts::ModelModule model_t,
          Concepts::PedsetModule pedset_t, Concepts::GoalsModule goals_t, Concepts::MapModule map_t>
class SimType {
  using base_output_t = decltype(std::declval<output_t>().write());
  using output_data_t = VIPRA::sim_output_t<output_t>;

 public:
  constexpr SimType(output_t&& output, model_t&& model, pedset_t&& pedset, goals_t&& goals, map_t&& obstacles)
      : _output(output), _model(model), _pedset(pedset), _goals(goals), _map(obstacles) {}

  /**
   * @brief Runs the simulation with the given parameters
   * 
   * @param params parameter module to use for the simulation
   * @return output_data_t Tuple of the output data
   */
  auto operator()(Concepts::ParamModule auto&& params) -> output_data_t {
    // TODO(rolland): this assumes that a only a single node should ever run this function, there may be sitations where this isn't user friendly?
    if (!ParameterSweep::is_root()) return {};

    params.get_input().load();

    if constexpr (std::is_same_v<output_data_t, void>) {
      run_sim(std::forward<decltype(params)>(params));
    } else {
      return run_sim(std::forward<decltype(params)>(params));
    }
  }

  // TODO(rolland): the simulation shouldn't have to know that it is being run in parallel
  //                  - this is needed because of the delayed loading of parameters, with the operator() every node would load the parameters when they've already been updated
  auto parallel_run(Concepts::ParamModule auto&& params) -> output_data_t {
    if constexpr (std::is_same_v<output_data_t, void>) {
      run_sim(std::forward<decltype(params)>(params));
    } else {
      return run_sim(std::forward<decltype(params)>(params));
    }
  }

  void set_sim_id(VIPRA::idx idx) { _currSimIdx = idx; }
  void add_sim_id(VIPRA::idx idx) { _currSimIdx += idx; }

 private:
  output_t                                _output;
  model_t                                 _model;
  pedset_t                                _pedset;
  goals_t                                 _goals;
  map_t                                   _map;
  BehaviorModel<pedset_t, map_t, goals_t> _behaviorModel;

  // TODO(rolland): NEXT create a sim id and update output to use it

  VIPRA::Random::Engine _engine{};

  VIPRA::idx      _currSimIdx{0};
  VIPRA::timestep _timestep{0};
  VIPRA::timestep _outputFrequency{0};

  auto run_sim(Concepts::ParamModule auto&& params) -> output_data_t {
    auto [maxTimestep, timestepSize, randomseed, state] = initialize(params);
    _output.new_run(_currSimIdx++);
    _timestep = 0;

    while (_timestep < maxTimestep && !_goals.is_sim_goal_met()) {
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

  /**
   * @brief Initializes the simulation and all of its modules with the given parameters
   * 
   * @param params parameter module to use for the simulation
   * @return auto Tuple of the max timestep, timestep size, random seed, and the initial state
   */
  auto initialize(Concepts::ParamModule auto& params) {
    register_params(params);

    VIPRA::timestep maxTimestep = params.template get_param<VIPRA::timestep>(Modules::Type::SIMULATION,
                                                                             "main", "max_timestep", _engine);
    VIPRA::timestep timestepSize = params.template get_param<VIPRA::timestep>(
        Modules::Type::SIMULATION, "main", "timestep_size", _engine);
    VIPRA::timestep randomseed =
        params.template get_param<VIPRA::timestep>(Modules::Type::SIMULATION, "main", "random_seed", _engine);

    _engine.reseed(randomseed);

    config(params);
    _map.initialize(_pedset);
    _goals.initialize(_pedset, _map);
    _model.initialize(_pedset, _map, _goals, _output, _engine);
    _behaviorModel.initialize(_pedset, _map, _goals, randomseed);

    VIPRA::State state;
    state.initialize(_pedset);

    return std::make_tuple(maxTimestep, timestepSize, randomseed, state);
  }

  /**
   * @brief Registers the parameters for the simulation and all of its modules
   * 
   * @param params parameter module to register the parameters with
   */
  void register_params(Concepts::ParamModule auto& params) {
    _output.register_params(params);
    _model.register_params(params);
    _pedset.register_params(params);
    _goals.register_params(params);
    _map.register_params(params);
    params.register_param(Modules::Type::SIMULATION, "main", "max_timestep");
    params.register_param(Modules::Type::SIMULATION, "main", "timestep_size");
    params.register_param(Modules::Type::SIMULATION, "main", "output_frequency");
    params.register_param(Modules::Type::SIMULATION, "main", "random_seed");
    BehaviorModel<pedset_t, map_t, goals_t>::register_params(params);
  }

  void config(Concepts::ParamModule auto& params) {
    _output.config(params, _engine);
    _model.config(params, _engine);
    _pedset.config(params, _engine);
    _goals.config(params, _engine);
    _map.config(params, _engine);
    _behaviorModel.config(params, _engine);
    _outputFrequency = params.template get_param<VIPRA::timestep>(Modules::Type::SIMULATION, "main",
                                                                  "output_frequency", _engine);
  }

  /**
   * @brief Adds pedestrian trajectories to the output
   * 
   */
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