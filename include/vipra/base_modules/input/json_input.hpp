#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <tuple>
#include <type_traits>
#include <utility>

#include <nlohmann/json.hpp>

#include "nlohmann/json_fwd.hpp"
#include "vipra/concepts/input.hpp"
#include "vipra/concepts/numeric.hpp"

#include "vipra/modules.hpp"
#include "vipra/types/float.hpp"
#include "vipra/types/parameter.hpp"
#include "vipra/util/get_nth_value.hpp"

#include "vipra/util/debug_do.hpp"
#include "vipra/util/has_tag.hpp"
#include "vipra/util/template_specialization.hpp"
#include "vipra/util/tuple_tail.hpp"

namespace VIPRA::Input {
class JSON {
 public:
  VIPRA_MODULE_TYPE(INPUT)
  explicit JSON(std::filesystem::path const& filepath) { load_file(filepath); }

  template <typename data_t, typename... key_ts>
  [[nodiscard]] auto get(key_ts&&... keys) const -> std::optional<data_t> {
    auto value = std::cref(_json);

    bool found = true;

    const auto findKey = [&](std::string const& key) {
      if (!found) return;

      if (!value.get().contains(key)) {
        found = false;
        return;
      }

      value = std::cref(value.get().at(key));
    };

    (findKey(keys), ...);

    if (!found) return std::nullopt;

    return get_value<data_t>(value);
  }

 private:
  nlohmann::json _json;

  void load_file(const std::filesystem::path& filepath) {
    if (!std::filesystem::exists(filepath))
      throw std::runtime_error("File does not exist at: " + filepath.string());

    if (!std::filesystem::is_regular_file(filepath))
      throw std::runtime_error("File is not a regular file at: " + filepath.string());

    std::ifstream file(filepath);
    if (!file.is_open()) throw std::runtime_error("Could not open file at: " + filepath.string());

    try {
      _json = nlohmann::json::parse(file);
    } catch (const nlohmann::json::parse_error& e) {
      throw std::runtime_error("Could not parse JSON file at: " + filepath.string() + "\n" + e.what());
    }

    file.close();
  }

  template <typename data_t>
  [[nodiscard]] auto get_value(const std::reference_wrapper<const nlohmann::json>& value) const
      -> std::optional<data_t> {
    if constexpr (std::is_same_v<data_t, VIPRA::Parameter<std::string>>) {
      return get_parameter(value);
    }

    else if constexpr (std::is_same_v<data_t, VIPRA::f3d>) {
      return get_f3d(value);
    }

    else if constexpr (std::is_same_v<data_t, VIPRA::f3dVec>) {
      return get_f3d_vec(value);
    }

    else if constexpr (Util::is_specialization<data_t, std::vector>::value) {
      using value_t = typename Util::get_specialization_internal<data_t>::type;
      return get_vector<value_t>(value);
    }

    else if constexpr (Util::is_specialization<data_t, Parameter>::value) {
      using value_t = typename Util::get_specialization_internal<data_t>::type;
      return numeric_parameter_helper<value_t>(value);
    } else {
      try {
        return value.get().get<data_t>();
      } catch (const nlohmann::json::type_error& e) {
        return std::nullopt;
      }
    }

    return std::nullopt;
  }

  [[nodiscard]] static auto is_f3d(nlohmann::json const& value) -> bool {
    if (!value.is_object()) return false;
    if (!value.contains("x") || !value.contains("y") || !value.contains("z")) return false;
    if (!value.at("x").is_number() || !value.at("y").is_number() || !value.at("z").is_number()) return false;
    return true;
  }

  [[nodiscard]] static auto is_f2d(nlohmann::json const& value) -> bool {
    if (!value.is_object()) return false;
    if (!value.contains("x") || !value.contains("y")) return false;
    if (!value.at("x").is_number() || !value.at("y").is_number()) return false;
    return true;
  }

  [[nodiscard]] static auto get_f3d(const std::reference_wrapper<const nlohmann::json>& value)
      -> std::optional<VIPRA::f3d> {
    if (!is_f3d(value.get())) return std::nullopt;

    return VIPRA::f3d(value.get().at("x").get<VIPRA::f_pnt>(), value.get().at("y").get<VIPRA::f_pnt>(),
                      value.get().at("z").get<VIPRA::f_pnt>());
  }

  [[nodiscard]] static auto get_f3d_vec(const std::reference_wrapper<const nlohmann::json>& value)
      -> std::optional<VIPRA::f3dVec> {
    if (!value.get().is_array()) return std::nullopt;

    VIPRA::f3dVec f3dVec;
    for (const auto& f3d : value.get()) {
      if (!is_f3d(f3d)) {
        if (!is_f2d(f3d)) return std::nullopt;

        f3dVec.emplace_back(f3d.at("x").get<VIPRA::f_pnt>(), f3d.at("y").get<VIPRA::f_pnt>(), 0);
        continue;
      }

      f3dVec.emplace_back(f3d.at("x").get<VIPRA::f_pnt>(), f3d.at("y").get<VIPRA::f_pnt>(),
                          f3d.at("z").get<VIPRA::f_pnt>());
    }

    return f3dVec;
  }

  template <typename data_t>
  [[nodiscard]] auto get_vector(const std::reference_wrapper<const nlohmann::json>& value) const
      -> std::optional<std::vector<data_t>> {
    if (!value.get().is_array()) return std::nullopt;

    std::vector<data_t> vec;
    for (const auto& element : value.get()) {
      auto elementValue = get_value<data_t>(std::cref(element));
      if (elementValue) vec.emplace_back(*elementValue);
    }

    return vec;
  }

  template <typename data_t>
  [[nodiscard]] static auto numeric_parameter_helper(
      std::reference_wrapper<const nlohmann::json> const& value) -> std::optional<Parameter<data_t>> {
    data_t inputData{};
    try {
      if (value.get().is_array()) {
        // discrete, choose random value
        Parameter<data_t> parameter{};
        parameter.randomType = Parameter<data_t>::RandomType::DISCRETE;
        // TODO(rolland): This needs to take a random value, currently just hard coded to 0 till randomization is added
        parameter.value = value.get()[0].template get<data_t>();
        return parameter;
      }

      if (value.get().is_object()) {
        // range, choose random value
        Parameter<data_t> parameter{};
        parameter.randomType = Parameter<data_t>::RandomType::RANGE;
        // TODO(rolland): This needs to take a random value, currently just hard coded to min till randomization is added
        parameter.value = value.get()["min"].template get<data_t>();
        return parameter;
      }

      // single value
      Parameter<data_t> parameter{};
      parameter.randomType = Parameter<data_t>::RandomType::NONE;
      parameter.value = value.get().template get<data_t>();
      return parameter;

    } catch (const nlohmann::json::type_error& e) {
      return std::nullopt;
    }
  }

  [[nodiscard]] static inline auto get_parameter(const std::reference_wrapper<const nlohmann::json>& value)
      -> std::optional<Parameter<std::string>> {
    std::string inputData{};
    try {
      if (value.get().is_array()) {
        // discrete, choose random value
        Parameter<std::string> parameter{};
        parameter.randomType = Parameter<std::string>::RandomType::DISCRETE;
        // TODO(rolland): This needs to take a random value, currently just hard coded to 0 till randomization is added
        parameter.value = value.get()[0].get<std::string>();
        return parameter;
      }

      Parameter<std::string> parameter{};
      parameter.randomType = Parameter<std::string>::RandomType::NONE;
      // TODO(rolland): This needs to take a random value, currently just hard coded to 0 till randomization is added
      parameter.value = value.get().get<std::string>();
      return parameter;
    } catch (const nlohmann::json::type_error& e) {
      return std::nullopt;
    }

    if (inputData.empty()) {
      return std::nullopt;
    }

    // single value
    Parameter<std::string> parameter{};
    parameter.randomType = Parameter<std::string>::RandomType::NONE;
    parameter.value = inputData;
    return parameter;
  }
};
}  // namespace VIPRA::Input

CHECK_MODULE(InputModule, VIPRA::Input::JSON)