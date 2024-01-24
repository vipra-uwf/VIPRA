#pragma once

#include <tuple>
#include <type_traits>
#include "vipra/concepts/output.hpp"
#include "vipra/types/idx.hpp"
#include "vipra/types/util/result_or_void.hpp"

namespace VIPRA::Module {
template <Concepts::OutputModule... output_ts>
class Output {
  VIPRA_MODULE_TYPE(OUTPUT)

  // TODO(rolland): decide if we need std::remove_reference

  template <typename output_t>
  // NOLINTNEXTLINE(readability-identifier-naming)
  struct write_helper {
    static auto write(output_t& output) {
      if constexpr (std::is_same_v<result_or_VOID_t<decltype(std::declval<output_t>().write())>, VOID>) {
        return VOID{};
      } else {
        return output.write();
      }
    }
  };

 public:
  constexpr explicit Output(output_ts... outputs) : _outputs(std::make_tuple(outputs...)) {}

  /**
   * @brief Calls write on all outputs, returning a tuple of the results or VOID
   * 
   * @return result_or_VOID_tuple<std::tuple<output_ts...>>::type 
   */
  auto write() -> std::tuple<result_or_VOID_t<decltype(std::declval<output_ts>().write())>...> {
    return std::apply(
        [](auto&&... outputs) { return std::make_tuple(write_helper<decltype(outputs)>::write(outputs)...); },
        _outputs);
  }

  /**
   * @brief Calls setup on all output modules
   * 
   * @param params 
   */
  void setup(const auto& params) {
    std::apply([&params](auto&&... outputs) { (outputs.setup(params), ...); }, _outputs);
  }

  /**
   * @brief Calls register_params on all output modules
   * 
   * @tparam params_t 
   */
  template <Concepts::ParamModule params_t>
  static void register_params() {
    (output_ts::template register_params<params_t>(), ...);
  }

  /**
   * @brief Calls sim_value on all output modules
   * 
   * @param key 
   * @param value 
   */
  void sim_value(const char* key, auto&& value) {
    std::apply([&key, &value](auto&&... outputs) { (outputs.sim_value(key, value), ...); }, _outputs);
  }

  /**
   * @brief Calls timestep_value on all output modules
   * 
   * @param key 
   * @param value 
   */
  void timestep_value(const char* key, auto&& value) {
    std::apply([&key, &value](auto&&... outputs) { (outputs.timestep_value(key, value), ...); }, _outputs);
  }

  /**
   * @brief Calls ped_value on all output modules
   * 
   * @param pedIdx 
   * @param key 
   * @param value 
   */
  void ped_value(VIPRA::idx pedIdx, const char* key, auto&& value) {
    std::apply([&pedIdx, &key, &value](auto&&... outputs) { (outputs.ped_value(pedIdx, key, value), ...); },
               _outputs);
  }

  /**
   * @brief Calls ped_timestep_value on all output modules
   * 
   * @param pedIdx 
   * @param key 
   * @param value 
   */
  void ped_timestep_value(VIPRA::idx pedIdx, const char* key, auto&& value) {
    std::apply(
        [&pedIdx, &key, &value](auto&&... outputs) { (outputs.ped_timestep_value(pedIdx, key, value), ...); },
        _outputs);
  }

 private:
  std::tuple<output_ts...> _outputs;
};

CHECK_MODULE(OutputModule, Output<Concepts::DummyOutput>);
}  // namespace VIPRA::Module
