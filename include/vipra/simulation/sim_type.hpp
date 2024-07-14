#pragma once

#include <cstddef>
#include <iostream>
#include <tuple>
#include <type_traits>
#include <utility>

#include "vipra/macros/parameters.hpp"

#include "vipra/modules/model.hpp"

#include "vipra/parameter_sweep/parameter_sweep.hpp"

#include "vipra/random/random.hpp"

// #include "vipra/special_modules/behavior_model.hpp"

#include "vipra/types/float.hpp"
#include "vipra/types/parameter.hpp"
#include "vipra/types/seed.hpp"
#include "vipra/types/state.hpp"
#include "vipra/types/time.hpp"
#include "vipra/types/util/result_or_void.hpp"
#include "vipra/types/util/sim_output.hpp"

#include "vipra/debug/debug_do.hpp"

#include "vipra/util/all_of_type.hpp"

// TODO(rolland): go through everything and handle errors more gracefully, currently we just throw

namespace VIPRA {

template <typename model_t, typename output_t, typename pedset_t, typename goals_t, typename obstacles_t>
class SimType : public Modules::Module<SimType<model_t, output_t, pedset_t, goals_t, obstacles_t>> {
  // type definitions
  using base_output_t = decltype(std::declval<output_t>().write());
  using output_data_t = VIPRA::sim_output_t<output_t>;
  using Modules::Module<SimType<model_t, output_t, pedset_t, goals_t, obstacles_t>>::config;
  using Modules::Module<SimType<model_t, output_t, pedset_t, goals_t, obstacles_t>>::register_params;

 public:
  VIPRA_MODULE_NAME("main");
  VIPRA_MODULE_TYPE(SIMULATION);

  VIPRA_REGISTER_PARAMS(VIPRA_PARAM("max_timestep", _maxTimestep),
                        VIPRA_PARAM("timestep_size", _timestepSize),
                        VIPRA_PARAM("output_frequency", _outputFrequency), VIPRA_PARAM("random_seed", _seed))

  template <typename pedinput_t, typename obsinput_t, typename params_t>
  auto operator()(pedinput_t&& pedInput, obsinput_t&& obsInput, params_t&& params) -> output_data_t;

  template <typename pedinput_t, typename obsinput_t, typename params_t>
  auto parallel_run(pedinput_t&& pedInput, obsinput_t&& obsInput, params_t&& params) -> output_data_t;

  void               set_sim_id(VIPRA::idx idx) { _currSimIdx = idx; }
  void               add_sim_id(VIPRA::idx idx) { _currSimIdx += idx; }
  [[nodiscard]] auto get_sim_id() -> VIPRA::idx { return _currSimIdx; }

 private:
  output_t    _output;
  model_t     _model;
  pedset_t    _pedset;
  goals_t     _goals;
  obstacles_t _map;
  // BehaviorModel<pedset_t, obstacles_t, goals_t> _behaviorModel;

  VIPRA::Random::Engine _engine{};

  VIPRA::idx      _currSimIdx{};
  VIPRA::timestep _currTimestep{};
  VIPRA::f_pnt    _timestepSize{};
  VIPRA::timestep _maxTimestep{};
  VIPRA::seed     _seed{};
  VIPRA::timestep _outputFrequency{};

  void register_step(auto&& params);
  void config_step(auto const& params);

  void initialize(auto const& pedInput, auto const& obsInput, auto&& params);

  auto run_sim(auto const& pedInput, auto const& obsInput, auto&& params) -> output_data_t;

  void output_positions();

 public:
  constexpr SimType(output_t&& output, model_t&& model, pedset_t&& pedset, goals_t&& goals,
                    obstacles_t&& obstacles)
      : _output(output), _model(model), _pedset(pedset), _goals(goals), _map(obstacles)
  {
  }
};

// ----------------------------------------------------------------------------------------------------------------
// --------------------------------------- IMPLEMENTATIONS --------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------

/**
   * @brief Runs the simulation with the given parameters
   * 
   * @param params parameter module to use for the simulation
   * @return output_data_t Tuple of the output data
   */
template <typename model_t, typename output_t, typename pedset_t, typename goals_t, typename obstacles_t>
template <typename pedinput_t, typename obsinput_t, typename params_t>
auto SimType<model_t, output_t, pedset_t, goals_t, obstacles_t>::operator()(pedinput_t&& pedInput,
                                                                            obsinput_t&& obsInput,
                                                                            params_t&&   params)
    -> output_data_t
{
  // TODO(rolland): this assumes that a only a single node should ever run this function, there may be sitations where this isn't user friendly?
  if ( ! ParameterSweep::is_root() ) return {};

  pedInput.load();
  obsInput.load();
  params.get_input().load();

  if constexpr ( std::is_same_v<output_data_t, void> ) {
    run_sim(std::forward<pedinput_t>(pedInput), std::forward<obsinput_t>(obsInput),
            std::forward<params_t>(params));
  }
  else {
    return run_sim(std::forward<pedinput_t>(pedInput), std::forward<obsinput_t>(obsInput),
                   std::forward<params_t>(params));
  }
}

// TODO(rolland): the simulation shouldn't have to know that it is being run in parallel
//                  - this is needed because of the delayed loading of parameters, with the operator() every node would load the parameters when they've already been updated
template <typename model_t, typename output_t, typename pedset_t, typename goals_t, typename obstacles_t>
template <typename pedinput_t, typename obsinput_t, typename params_t>
auto SimType<model_t, output_t, pedset_t, goals_t, obstacles_t>::parallel_run(pedinput_t&& pedInput,
                                                                              obsinput_t&& obsInput,
                                                                              params_t&&   params)
    -> output_data_t
{
  if constexpr ( std::is_same_v<output_data_t, void> ) {
    run_sim(std::forward<pedinput_t>(pedInput), std::forward<obsinput_t>(obsInput),
            std::forward<params_t>(params));
  }
  else {
    return run_sim(std::forward<pedinput_t>(pedInput), std::forward<obsinput_t>(obsInput),
                   std::forward<params_t>(params));
  }
}

template <typename model_t, typename output_t, typename pedset_t, typename goals_t, typename obstacles_t>
template <typename pedinput_t, typename obsinput_t, typename params_t>
auto SimType<model_t, output_t, pedset_t, goals_t, obstacles_t>::run_sim(pedinput_t const& pedInput,
                                                                         obsinput_t const& obsInput,
                                                                         params_t&& params) -> output_data_t
{
  // initialize parameters, etc
  register_step(std::forward<params_t>(params));

  initialize(pedInput, obsInput, std::forward<params_t>(params));

  _output.new_run(_currSimIdx++);

  _currTimestep = 0;

  VIPRA::State state;
  state.initialize(_pedset);

  // main loop
  while ( _currTimestep < _maxTimestep && ! _goals.is_sim_goal_met() ) {
    _model.timestep(_pedset, _map, _goals, _output, state, _timestepSize, _currTimestep);
    // _behaviorModel.timestep(_pedset, _map, _goals, state, timestepSize);
    _pedset.update(state);
    _goals.update(_pedset, _map, _timestepSize);
    output_positions();
    ++_currTimestep;
  }

  // write output and if output returns a value, return that
  if constexpr ( std::is_same_v<output_data_t, void> ) {
    _output.write();
  }
  else {
    return _output.write();
  }
}

template <typename model_t, typename output_t, typename pedset_t, typename goals_t, typename obstacles_t>
template <typename params_t>
void SimType<model_t, output_t, pedset_t, goals_t, obstacles_t>::register_step(params_t&& params)
{
  register_params(params);
  _output.register_params(std::forward<params_t>(params));
  _model.register_params(std::forward<params_t>(params));
  _pedset.register_params(std::forward<params_t>(params));
  _goals.register_params(std::forward<params_t>(params));
  _map.register_params(std::forward<params_t>(params));
}

/**
   * @brief Initializes the simulation and all of its modules with the given parameters
   * 
   * @param params parameter module to use for the simulation
   * @return auto Tuple of the max timestep, timestep size, random seed, and the initial state
   */
template <typename model_t, typename output_t, typename pedset_t, typename goals_t, typename obstacles_t>
template <typename pedinput_t, typename obsinput_t, typename params_t>
void SimType<model_t, output_t, pedset_t, goals_t, obstacles_t>::initialize(pedinput_t const& pedInput,
                                                                            obsinput_t const& obsInput,
                                                                            params_t&&        params)
{
  config_step(std::forward<params_t>(params));

  _engine.reseed(_seed + (_currSimIdx * _currSimIdx));

  _map.initialize(obsInput);
  _pedset.initialize(pedInput, _map);
  _goals.initialize(_pedset, _map);
  _model.initialize(_pedset, _map, _goals, _output, _engine);
  // _behaviorModel.initialize(_pedset, _map, _goals, randomseed);
}

template <typename model_t, typename output_t, typename pedset_t, typename goals_t, typename obstacles_t>
template <typename params_t>
void SimType<model_t, output_t, pedset_t, goals_t, obstacles_t>::config_step(params_t const& params)
{
  config(params, _engine);
  _output.config(params, _engine);
  _model.config(params, _engine);
  _pedset.config(params, _engine);
  _goals.config(params, _engine);
  _map.config(params, _engine);
  // _behaviorModel.config(params, _engine);
}

/**
   * @brief Adds pedestrian trajectories to the output
   * 
   */
template <typename model_t, typename output_t, typename pedset_t, typename goals_t, typename obstacles_t>
void SimType<model_t, output_t, pedset_t, goals_t, obstacles_t>::output_positions()
{
  if ( _currTimestep % _outputFrequency != 0 ) {
    return;
  }

  const VIPRA::size pedCnt = _pedset.num_pedestrians();
  auto const&       coords = _pedset.all_coords();

  for ( VIPRA::idx i = 0; i < pedCnt; ++i ) {
    _output.ped_timestep_value(i, _currTimestep / _outputFrequency, "position", coords.at(i));
  }
}

}  // namespace VIPRA
