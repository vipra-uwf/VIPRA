#pragma once

#include <type_traits>

#include "vipra/concepts/parameters.hpp"
#include "vipra/simulation/module_checks.hpp"
#include "vipra/simulation/sim_type.hpp"
#include "vipra/util/find_index.hpp"

namespace VIPRA {
template <typename... args_t>
constexpr auto simulation(args_t... args) {
  // Finds the index for each module type
  constexpr std::size_t OUTPUT_IDX = Util::FindIndex<0, Checks::OutputCoord, args_t...>::value;
  constexpr std::size_t MODEL_IDX = Util::FindIndex<0, Checks::Model, args_t...>::value;
  constexpr std::size_t PEDSET_IDX = Util::FindIndex<0, Checks::Pedset, args_t...>::value;
  constexpr std::size_t GOALS_IDX = Util::FindIndex<0, Checks::Goals, args_t...>::value;
  constexpr std::size_t MAP_IDX = Util::FindIndex<0, Checks::Map, args_t...>::value;

  // Static asserts to ensure that the modules are valid
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
  static_assert(MAP_IDX != -1,
                "Map Module does Not conform to the Map Module specification OR A valid Map "
                "Module was not provided");

  auto&& temp = std::forward_as_tuple(args...);

  // Returns the SimType object
  auto sim = SimType(std::move(std::get<OUTPUT_IDX>(temp)), std::move(std::get<MODEL_IDX>(temp)),
                     std::move(std::get<PEDSET_IDX>(temp)), std::move(std::get<GOALS_IDX>(temp)),
                     std::move(std::get<MAP_IDX>(temp)));

  // sim.reconfig();

  return sim;
}
}  // namespace VIPRA