#include <cstddef>
#include <iostream>
#include <tuple>
#include <type_traits>
#include <utility>

#include "vipra/concepts/all_concepts.hpp"

#include "vipra/concepts/parameters.hpp"
#include "vipra/types/parameter.hpp"
#include "vipra/types/time.hpp"
#include "vipra/types/util/result_or_void.hpp"

#include "vipra/util/all_of_type.hpp"
#include "vipra/util/debug_do.hpp"

// TODO(rolland): go through everything and handle errors more gracefully, currently we just throw

namespace VIPRA {

enum class Mode { SINGLE, SWEEP };

template <Concepts::ParamModule params_t, Concepts::OutputModule output_t, Concepts::ModelModule model_t,
          Concepts::PedsetModule pedset_t, Concepts::GoalsModule goals_t, Concepts::MapModule map_t>
class SimType {
  using base_output_t = decltype(std::declval<output_t>().write());
  using output_data_t =
      std::conditional_t<std::is_same_v<base_output_t, void> || std::is_same_v<base_output_t, VOID> ||
                             Util::all_of_type_v<VOID, base_output_t>,
                         void, decltype(std::declval<output_t>().write())>;

 public:
  constexpr SimType(Mode mode, params_t&& params, output_t&& output, model_t&& model, pedset_t&& pedset,
                    goals_t&& goals, map_t&& obstacles)
      : _mode(mode),
        _params(params),
        _output(output),
        _model(model),
        _pedset(pedset),
        _goals(goals),
        _map(obstacles) {}

  static void register_params(Concepts::ParamModule auto& params) {
    params.register_param(Modules::Type::SIMULATION, "main", "max_timestep", ParameterType::REQUIRED);
    params.register_param(Modules::Type::SIMULATION, "main", "timestep_size", ParameterType::REQUIRED);
    params.register_param(Modules::Type::SIMULATION, "main", "output_frequency", ParameterType::REQUIRED);
  }

  auto operator()() -> output_data_t {
    if constexpr (std::is_same_v<output_data_t, void>) {
      run_sim();
    } else {
      return run_sim();
    }
  }

  [[nodiscard]] auto run_sim() -> output_data_t {
    const auto [maxTimestep, timestepSize] = get_sim_params();

    _model.initialize(_pedset);
    _map.initialize(_pedset);
    _goals.initialize(_pedset, _map);

    Util::debug_do([]() { std::cout << "Simulation Starting" << std::endl; });

    // TODO(rolland): modules have no way of reaching output except through simulation, we need to fix this

    while (_timestep < maxTimestep) {
      const VIPRA::State& state = _model.timestep(_pedset, _map, _goals, timestepSize);
      _pedset.update(state);
      _goals.update(_pedset, _map);
      output_positions();
      ++_timestep;
    }

    Util::debug_do([]() { std::cout << "Simulation complete" << std::endl; });

    if constexpr (std::is_same_v<output_data_t, void>) {
      _output.write();
    } else {
      return _output.write();
    }
  }

 private:
  Mode     _mode{Mode::SINGLE};
  params_t _params;
  output_t _output;
  model_t  _model;
  pedset_t _pedset;
  goals_t  _goals;
  map_t    _map;

  VIPRA::timestep _timestep{0};
  VIPRA::timestep _outputFrequency{0};

  [[nodiscard]] constexpr auto get_sim_params() -> std::tuple<VIPRA::f_pnt, VIPRA::f_pnt> {
    VIPRA::timestep maxTimestep =
        _params.template get_param<VIPRA::timestep>(Modules::Type::SIMULATION, "max_timestep");
    VIPRA::f_pnt timestepSize =
        _params.template get_param<VIPRA::f_pnt>(Modules::Type::SIMULATION, "timestep_size");
    VIPRA::timestep outputFrequency =
        _params.template get_param<VIPRA::timestep>(Modules::Type::SIMULATION, "output_frequency");

    if (outputFrequency == 0) throw std::runtime_error("Output frequency must be greater than 0");
    _outputFrequency = outputFrequency;

    return {maxTimestep, timestepSize};
  }

  void output_positions() {
    if (_timestep % _outputFrequency != 0) {
      return;
    }

    const VIPRA::size pedCnt = _pedset.num_pedestrians();
    const auto&       coords = _pedset.all_coords();

    for (VIPRA::idx i = 0; i < pedCnt; ++i) {
      _output.ped_timestep_value(i, _timestep / _outputFrequency, "position", coords.at(i));
    }
  }
};
}  // namespace VIPRA