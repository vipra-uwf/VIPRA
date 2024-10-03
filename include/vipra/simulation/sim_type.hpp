#pragma once

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <string>

#include "vipra/input/json.hpp"
#include "vipra/macros/parameters.hpp"

#include "vipra/modules.hpp"
#include "vipra/modules/map_input.hpp"
#include "vipra/modules/model.hpp"
#include "vipra/modules/output.hpp"
#include "vipra/modules/pedestrian_input.hpp"

#include "vipra/modules/pedestrians.hpp"
#include "vipra/random/random.hpp"

#include "vipra/simulation/module_loading.hpp"
#include "vipra/special_modules/behavior_model.hpp"

#include "vipra/special_modules/output.hpp"
#include "vipra/special_modules/parameters.hpp"
#include "vipra/types/float.hpp"
#include "vipra/types/seed.hpp"
#include "vipra/types/state.hpp"
#include "vipra/types/time.hpp"

// TODO(rolland): go through everything and handle errors more gracefully, currently we just throw

namespace VIPRA {

class Simulation : public Modules::Module<Simulation> {
 public:
  VIPRA_MODULE_NAME("main");
  VIPRA_MODULE_TYPE(Simulation);

  VIPRA_REGISTER_PARAMS(VIPRA_PARAM("max_timestep", _maxTimestep),
                        VIPRA_PARAM("timestep_size", _timestepSize),
                        VIPRA_PARAM("output_frequency", _outputFrequency),
                        VIPRA_PARAM("random_seed", _seed),
                        VIPRA_PARAM("output_params", _outputParams));

  void set_modules(std::string const& modulesPath)
  {
    Input::JSON input{};

    input.load(modulesPath);

    std::array<Modules::Type, Modules::MODULE_COUNT> modules{
        Modules::Type::Model,   Modules::Type::Goals,
        Modules::Type::Map,     Modules::Type::Pedestrians,
        Modules::Type::Output,  Modules::Type::PedInput,
        Modules::Type::MapInput};

    for ( auto type : modules ) {
      auto module = input.get<std::string>({to_string(type)});

      if ( ! module ) {
        throw std::runtime_error("No " + to_string(type) + " Module Provided");
      }

      VIPRA::Log::debug("Config: {} = {}", to_string(type), module.value());

      set_module(type, module.value());
    }
  }

  void run_sim(Parameters& params);
  void operator()(std::string const& pedPath, std::string const& mapPath,
                  std::string const& paramsPath);

  void set_install_dir(std::string const& dir) { _installDir = dir; }

  void               set_sim_id(VIPRA::idx idx) { _currSimIdx = idx; }
  void               add_sim_id(VIPRA::idx idx) { _currSimIdx += idx; }
  [[nodiscard]] auto get_sim_id() const -> VIPRA::idx { return _currSimIdx; }

  auto get_ped_input() -> Modules::PedestrianInput& { return *_pedInput; }
  auto get_map_input() -> Modules::MapInput& { return *_mapInput; }

  void set_module(Modules::Type type, std::string const& name)
  {
    const auto set = [&](auto& module) {
      auto [mod, config] =
          load_module<decltype(*module.get())>(name, _installDir, type);
      module = std::move(mod);
      _configs[type] = std::move(config);
    };

    if ( type == Modules::Type::Output ) {
      auto [mod, config] =
          load_module<Modules::Output>(name, _installDir, type);
      _output.add_output(std::move(mod), std::move(config));

      return;
    }

    switch ( type ) {
      case Modules::Type::Model:
        set(_model);
        break;
      case Modules::Type::Goals:
        set(_goals);
        break;
      case Modules::Type::Pedestrians:
        set(_pedset);
        break;
      case Modules::Type::Map:
        set(_map);
        break;
      case Modules::Type::PedInput:
        if ( name == "JSON" ) {
          _pedInput = std::make_unique<VIPRA::Input::JSON>();
          _configs[type] = [](void*, Parameters&, VIPRA::Random::Engine&) {};
          break;
        }
        set(_pedInput);
        break;
      case Modules::Type::MapInput:
        if ( name == "JSON" ) {
          _mapInput = std::make_unique<VIPRA::Input::JSON>();
          _configs[type] = [](void*, Parameters&, VIPRA::Random::Engine&) {};
          break;
        }
        set(_mapInput);
        break;
      case Modules::Type::Output:
      case Modules::Type::Parameters:
      case Modules::Type::Simulation:
      case Modules::Type::Behavior_model:
        throw std::runtime_error("Can't Load " + to_string(type) + " Modules");
    }
  }

 private:
  CoordModules::OutputCoordinator           _output;
  std::unique_ptr<Modules::Model>           _model;
  std::unique_ptr<Modules::Pedestrians>     _pedset;
  std::unique_ptr<Modules::Goals>           _goals;
  std::unique_ptr<Modules::Map>             _map;
  std::unique_ptr<Modules::PedestrianInput> _pedInput;
  std::unique_ptr<Modules::MapInput>        _mapInput;
  BehaviorModel                             _behaviorModel;

  std::map<Modules::Type,
           std::function<void(void*, Parameters&, VIPRA::Random::Engine&)>>
      _configs;

  VIPRA::Random::Engine _engine;

  VIPRA::idx      _currSimIdx{};
  VIPRA::timestep _currTimestep{};
  VIPRA::f_pnt    _timestepSize{};
  VIPRA::timestep _maxTimestep{};
  VIPRA::seed     _seed{};
  VIPRA::timestep _outputFrequency{};
  std::string     _installDir;
  bool            _outputParams{false};

  void initialize(Parameters& params);
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
inline void Simulation::operator()(std::string const& pedPath,
                                   std::string const& mapPath,
                                   std::string const& paramsPath)
{
  // TODO(rolland): issue #25 this assumes that a only a single node should ever run this function, there may be sitations where this isn't user friendly?
  // if ( ! ParameterSweep::is_root() ) return;

  _pedInput->load(pedPath);
  _mapInput->load(mapPath);

  Parameters params;
  params.get_input().load(paramsPath);

  run_sim(params);
}

// TODO(rolland): issue #25 the simulation shouldn't have to know that it is being run in parallel
//                  - this is needed because of the delayed loading of parameters, with the operator() every node would load the parameters when they've already been updated
inline void Simulation::run_sim(Parameters& params)
{
  params.reset();

  initialize(params);

  _output.new_run(_currSimIdx++);

  _currTimestep = 0;

  VIPRA::State state;
  state.initialize(*_pedset);

  // main loop
  while ( _currTimestep < _maxTimestep && ! _goals->is_sim_goal_met() ) {
    _model->timestep(*_pedset, *_map, *_goals, state, _timestepSize,
                     _currTimestep);
    _behaviorModel.timestep(*_pedset, *_map, *_goals, state, _timestepSize);
    _pedset->update(state);
    _goals->update(*_pedset, *_map, _timestepSize);

    if ( _currTimestep % _outputFrequency == 0 )
      _output.timestep_update(_currTimestep, _timestepSize, state);

    ++_currTimestep;
  }

  if ( _outputParams )
    _output.write_to_file("parameters.json", params.get_used_parameters());

  _output.write();
}

/**
   * @brief Initializes the simulation and all of its modules with the given parameters
   * 
   * @param params parameter module to use for the simulation
   * @return auto Tuple of the max timestep, timestep size, random seed, and the initial state
   */
inline void Simulation::initialize(Parameters& params)
{
  register_params(params);
  config(params, _engine);

  _output.config(params, _engine);

  _configs[Modules::Type::Model](_model.get(), params, _engine);
  _configs[Modules::Type::Pedestrians](_pedset.get(), params, _engine);
  _configs[Modules::Type::Goals](_goals.get(), params, _engine);
  _configs[Modules::Type::Map](_map.get(), params, _engine);
  _configs[Modules::Type::PedInput](_pedInput.get(), params, _engine);
  _configs[Modules::Type::MapInput](_mapInput.get(), params, _engine);

  _pedset->register_base_params(params);
  _pedset->config_base(params, _engine);

  _behaviorModel.register_params(params);
  _behaviorModel.config(params, _engine);

  _engine.reseed(_seed + (_currSimIdx * _currSimIdx));

  _map->initialize(*_mapInput);
  _pedset->initialize(*_pedInput, *_map, _engine);
  _goals->initialize(*_pedset, *_map, _engine);
  _model->initialize(*_pedset, *_map, *_goals, _engine);
  _behaviorModel.initialize(*_pedset, *_map, *_goals, _seed);
}
}  // namespace VIPRA
