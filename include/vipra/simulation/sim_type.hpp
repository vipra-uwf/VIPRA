#pragma once

#include <memory>
#include <string>

#include "vipra/macros/parameters.hpp"

#include "vipra/modules.hpp"
#include "vipra/modules/collision_avoidance.hpp"
#include "vipra/modules/map_input.hpp"
#include "vipra/modules/model.hpp"
#include "vipra/modules/pedestrian_input.hpp"
#include "vipra/modules/pedestrians.hpp"

#include "vipra/random/random.hpp"

#include "vipra/special_modules/behavior_model.hpp"
#include "vipra/special_modules/output.hpp"
#include "vipra/special_modules/parameters.hpp"

#include "vipra/types/float.hpp"
#include "vipra/types/seed.hpp"
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

  void run_sim(Parameters& params);
  void operator()(std::string const& pedPath, std::string const& mapPath,
                  std::string const& paramsPath);

  void set_install_dir(std::string const& dir) { _installDir = dir; }
  void set_module(Modules::Type type, std::string const& name);
  void set_modules(std::string const& modulesPath);

  void               set_sim_id(VIPRA::idx idx) { _currSimIdx = idx; }
  void               add_sim_id(VIPRA::idx idx) { _currSimIdx += idx; }
  [[nodiscard]] auto get_sim_id() const -> VIPRA::idx { return _currSimIdx; }

  auto get_ped_input() -> Modules::PedestrianInput& { return *_pedInput; }
  auto get_map_input() -> Modules::MapInput& { return *_mapInput; }

 private:
  CoordModules::OutputCoordinator              _output;
  std::unique_ptr<Modules::Model>              _model;
  std::unique_ptr<Modules::Pedestrians>        _pedset;
  std::unique_ptr<Modules::Goals>              _goals;
  std::unique_ptr<Modules::Map>                _map;
  std::unique_ptr<Modules::CollisionAvoidance> _collision;
  std::unique_ptr<Modules::PedestrianInput>    _pedInput;
  std::unique_ptr<Modules::MapInput>           _mapInput;
  BehaviorModel                                _behaviorModel;

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
}  // namespace VIPRA
