#pragma once

#include <type_traits>

#include "vipra/types/util/result_or_void.hpp"
#include "vipra/util/all_of_type.hpp"

namespace VIPRA {
/**
 * @brief Helper struct to get the type of the output of the write function of the output_t
 * 
 * @tparam output_t 
 */
template <typename output_t>
// NOLINTNEXTLINE(readability-identifier-naming) lowercase is a regular naming convetion here
struct sim_output {
  using type = std::conditional_t<std::is_same_v<output_t, void> ||
                                      std::is_same_v<output_t, VOID> ||
                                      Util::all_of_type_v<VOID, output_t>,
                                  void, decltype(std::declval<output_t>().write())>;
};

template <typename output_t>
using sim_output_t = typename sim_output<output_t>::type;
}  // namespace VIPRA