#include <cstddef>
#include <iostream>
#include <tuple>
#include <type_traits>
#include <utility>

#include "vipra/concepts/all_concepts.hpp"

#include "vipra/types/parameter.hpp"
#include "vipra/types/time.hpp"
#include "vipra/types/util/result_or_void.hpp"

#include "vipra/util/all_of_type.hpp"
#include "vipra/util/debug_do.hpp"

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

  static void register_params() {
    params_t::register_param(Modules::Type::SIMULATION, "max_timestep", ParameterType::REQUIRED);
    params_t::register_param(Modules::Type::SIMULATION, "timestep_size", ParameterType::REQUIRED);
    output_t::template register_params<params_t>();
    model_t::template register_params<params_t>();
    pedset_t::template register_params<params_t>();
    goals_t::template register_params<params_t>();
    map_t::template register_params<params_t>();
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

    while (_timestep < maxTimestep) {
      const VIPRA::State& state = _model.timestep(_pedset, _map, _goals, timestepSize);
      _pedset.update(state);
      _output.current_state(state);
      _goals.update(_pedset, _map);
      ++_timestep;
    }

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

  [[nodiscard]] constexpr auto get_sim_params() -> std::pair<VIPRA::f_pnt, VIPRA::f_pnt> {
    VIPRA::timestep maxTimestep =
        _params.template get_param<VIPRA::timestep>(Modules::Type::SIMULATION, "max_timestep");
    VIPRA::f_pnt timestepSize =
        _params.template get_param<VIPRA::f_pnt>(Modules::Type::SIMULATION, "timestep_size");

    return {maxTimestep, timestepSize};
  }
};
}  // namespace VIPRA