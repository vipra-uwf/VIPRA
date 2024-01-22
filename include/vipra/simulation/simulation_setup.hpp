#pragma once

#include "vipra/base_modules.hpp"
#include "vipra/simulation/module_checks.hpp"
#include "vipra/simulation/sim_type.hpp"

namespace VIPRA {
template <std::size_t index_t, template <typename> typename check_t, typename... types_t>
struct FindIndex;

template <std::size_t index_t, template <typename> typename check_t, typename type_t, typename... types_t>
struct FindIndex<index_t, check_t, type_t, types_t...> {
  // NOLINTNEXTLINE(readability-identifier-naming) lowercase is a regular naming convetion here
  static constexpr std::size_t value =
      check_t<type_t>::value ? index_t : FindIndex<index_t + 1, check_t, types_t...>::value;
};

template <std::size_t index_t, template <typename> typename check_t>
struct FindIndex<index_t, check_t> {
  // NOLINTNEXTLINE(readability-identifier-naming) lowercase is a regular naming convetion here
  static constexpr std::size_t value = -1;
};

template <typename... args_t>
constexpr auto simulation(Mode mode, args_t... args) {
  constexpr std::size_t PARAMS_IDX = FindIndex<0, Checks::Params, args_t...>::value;
  constexpr std::size_t OUTPUT_IDX = FindIndex<0, Checks::Output, args_t...>::value;
  constexpr std::size_t MODEL_IDX = FindIndex<0, Checks::Model, args_t...>::value;
  constexpr std::size_t PEDSET_IDX = FindIndex<0, Checks::Pedset, args_t...>::value;
  constexpr std::size_t GOALS_IDX = FindIndex<0, Checks::Goals, args_t...>::value;
  constexpr std::size_t OBSTACLES_IDX = FindIndex<0, Checks::Obstacles, args_t...>::value;

  static_assert(PARAMS_IDX != -1,
                "Params Module does Not conform to the Params Module specification OR A valid Params Module "
                "was not provided");
  static_assert(OUTPUT_IDX != -1,
                "Output Module does Not conform to the Output Module specification OR A valid Output Module "
                "was not provided");
  static_assert(MODEL_IDX != -1,
                "Model Module does Not conform to the Model Module specification OR A valid Model Module was "
                "not provided");
  static_assert(PEDSET_IDX != -1,
                "Pedset Module does Not conform to the Pedset Module specification OR A valid Pedset Module "
                "was not provided");
  static_assert(GOALS_IDX != -1,
                "Goals Module does Not conform to the Goals Module specification OR A valid Goals Module was "
                "not provided");
  static_assert(OBSTACLES_IDX != -1,
                "Obstacle Module does Not conform to the Obstacle Module specification OR A valid Obstacle "
                "Module was not provided");

  auto&& temp = std::forward_as_tuple(args...);

  return SimType(mode, std::move(std::get<PARAMS_IDX>(temp)), std::move(std::get<OUTPUT_IDX>(temp)),
                 std::move(std::get<MODEL_IDX>(temp)), std::move(std::get<PEDSET_IDX>(temp)),
                 std::move(std::get<GOALS_IDX>(temp)), std::move(std::get<OBSTACLES_IDX>(temp)));
}
}  // namespace VIPRA